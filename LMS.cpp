#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>
using namespace std;

//Note: All the books are inside a directory, whose name is taken as input from the user

//function that checks if a string ends with another string
bool hasEnding (string fullString, string ending);

//function that is used to read a directory
string read_directory(); 

//function to display all the available files which are named in "index.txt"
void display_files();

//function to search for a book in the directory via its title or its author name
void search_files(string directory);

//function that prints a book to the console
void print_book(vector <string> book, string directory);

//function that performs different analytics on the Books (Play and Novel). Can be extended to other genres as well
void analytics(vector <string> book, string directory);

//function that gives the complete filename of the book chosen
string get_filename(vector <string> book, string directory);

//class for Book
class Book
{
    protected:
    string filename, input;
    public:

    //constructor
    Book(string filename_)
    {
        filename = filename_;
    }

    //setter function for input
    string set_input()
    {
        cin>>input;
        return input;
    }

    //getter function for filename
    string get_filename()
    {
        return filename;
    }
};

//subclass of class Book for Novel
class Novel: public Book
{
    int para_chapter, chapter_number = 0;;
    int chapter_array[5] = {0, 0, 0, 0, 0};

    public:

    //constructor
    Novel(string filename) : Book(filename){}

    //function for choosing whether to work on a paragraph or a chapter
    int para_or_chap()
    {
        cout<<"Enter 1 for looking in paragraphs\nEnter 2 for looking in chapters"<<endl;
        cin>>para_chapter;
        return para_chapter;
    }

    //function that changes the rankings of the top 5 paragraphs or chapters
    void transfer(vector <vector<string>> &top, int top_count[], int limit, vector <string> para, int count) 
    {
        for (int i = 4; i > limit; i--)
        {
            top_count[i] = top_count[i-1];
            top[i] = top[i-1];
            chapter_array[i] = chapter_array[i-1];
        }
        top_count[limit] = count;
        top[limit] = para;
        chapter_array[limit] = chapter_number;
    }

    //function that finds the top 5 paragraphs or chapters
    void counter()
    {
        vector <string> para;
        vector <vector <string>> top
        {
            {""}, {""}, {""}, {""}, {""}
        };
        int count = 0;
        int top_count[5] = {0, 0, 0, 0, 0};
        string line;
        
        fstream book_novel;
        book_novel.open(filename, ios::in);

        while (!book_novel.eof())
        {
            getline(book_novel, line);

            if ((para_chapter == 1 && line == "") || (para_chapter == 2 && line.substr(0, 7) == "CHAPTER"))
            {                
                if (line.substr(0, 7) == "CHAPTER")
                    chapter_number++;

                if (top_count[0] < count)
                    transfer(top, top_count, 0, para, count);

                else if (top_count[1] < count)
                    transfer(top, top_count, 1, para, count);

                else if (top_count[2] < count)
                    transfer(top, top_count, 2, para, count);

                else if (top_count[3] < count)
                    transfer(top, top_count, 3, para, count);

                else if (top_count[4] < count)
                    transfer(top, top_count, 4, para, count);

                para.clear();
                count = 0;
            }

            else
            {
                para.push_back(line);
                istringstream iss(line);
                do
                {
                    string subs;
                    iss >> subs;
                    char end_char = subs[subs.length()-1];
                    if (end_char == '.' || end_char == ',' || end_char == '!' || end_char == ';' || end_char == ':' || end_char == '?' || end_char == '&')
                        subs = subs.substr(0, subs.length()-1);
                    transform(subs.begin(), subs.end(), subs.begin(), ::tolower); 
                    transform(input.begin(), input.end(), input.begin(), ::tolower); 
                    if (subs == input)
                        count++;
                }
                while (iss);
            }
        }

        chapter_number++;
        if (top_count[0] < count)
            transfer(top, top_count, 0, para, count);

        else if (top_count[1] < count)
            transfer(top, top_count, 1, para, count);

        else if (top_count[2] < count)
            transfer(top, top_count, 2, para, count);

        else if (top_count[3] < count)
            transfer(top, top_count, 3, para, count);

        else if (top_count[4] < count)
            transfer(top, top_count, 4, para, count);

        string title;
        if (para_chapter == 1)
            title = "Paragraph";
        else
            title = "Chapter";

        cout<<endl;
        for (int i = 0; i < 5; i++)
        {
            cout<<title<<" at rank "<<i+1<<" for number of matching words"<<endl;
            if (para_chapter == 2)
                cout<<"Chapter Number: ["<<chapter_array[i]-1<<"]"<<endl;

            // for (int j = 0; j < top[i].size(); j++)
            // {
            //     cout<<top[i][j]<<endl;
            // }

            //*************************************************
            //uncomment the above 4 lines only if you want to display the chapter too!
            //*************************************************

            cout<<"Number of words matched: "<<top_count[i]<<endl<<endl;
        }
    }
};

//subclass of class Book for Play
class Play: public Book
{
    vector <vector<vector<string>>> vec;

    public:
    //constructor
    Play(string filename) : Book(filename){} 

    //function to read in all the characters that appear in the play scenewise
    void reader()
    {
        int act_number = 0, scene_number = 0;
        vector<vector<string>> act;
        vector<string> scene;
        fstream play_book;
        play_book.open(filename, ios::in);

        while (!play_book.eof())
        {
            string line;
            getline(play_book, line);
            int flag = 1;
            int caps = 1;
            int i;
            for (i = 0; i < line.length(); i++)
            {
                if (line[i] == '.')
                    break;
            }

            for (int j = 0; j < i; j++)
            {
                if (islower(line[j]))
                {
                    flag = 0;
                    break;
                }
            }

            if (flag == 1)
                line = line.substr(0, i+1);

            if ((flag || line.find("SCENE") != string::npos) && line.length() != 0)
            {
                //3 cases->its an act, its a scene, its characters
                if (line.find("ACT") != string::npos)
                {
                    vec.push_back(act);
                    act.clear();
                    act_number++;
                    scene_number = 0;
                    continue;
                }

                if (line.find("SCENE") != string::npos)
                {
                    act.push_back(scene);
                    scene.clear();
                    scene_number++;
                    continue;
                }

                if (line[0] != '*' && act_number)
                {
                    scene.push_back(line.substr(0, line.length()-1));
                }
            }
        }
        act.push_back(scene);
        vec.push_back(act);
    }

    //function to find the other characters that appear along with the input character in the same scene
    void counter()
    {
        vector <string> characters;
        transform(input.begin(), input.end(), input.begin(), ::toupper); 

        for (auto i = vec.begin(); i != vec.end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); j++)
            {
                if (count(j->begin(), j->end(), input))
                {
                    for (auto k = j->begin(); k != j->end(); k++)
                    {
                        characters.push_back(*k);
                    }
                }
            }
        }
        
        set<string> s; 

        for (int i = 0; i < characters.size(); i++)
        {
            s.insert(characters[i]);
        }
        s.erase(input);
  
        cout<<"Characters that appeared with "<<input<<" in at least one scene:"<<endl;
        for (auto i = s.begin(); i != s.end(); i++)
        {
            cout<<*i<<endl;
        }

        if (s.size() == 0)
            cout<<"No Characters appeared with "<<input<<endl;
    }
};


//main function
int main(void)
{
    string directory = read_directory();
    display_files();
    search_files(directory);
}

//function that checks if a string ends with another string
bool hasEnding (string fullString, string ending) 
{
    if (fullString.length() >= ending.length())
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    else
        return false;
}

//function that is used to read a directory
string read_directory()
{
    DIR *dr;
    struct dirent *en;

    cout<<"Enter the name of the directory"<<endl;
    string directory;
    cin>>directory;
    string first = "./";
    directory = first + directory + first[1];
    dr = opendir(directory.c_str()); //open all directory
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            if (hasEnding(en->d_name, ".txt"))
            {
                fstream file;
                file.open("index.txt", ios::in);
                if (!file)
                    file.open("index.txt", ios::out);

                string line;
                int flag = 0;
                while(!file.eof())
                {
                    getline(file, line);
                    if (hasEnding(line, en->d_name))
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    cout<<"New book found"<<endl<<"Name: "<<en->d_name<<endl<<"What is the type of this book? (Novel or Play)"<<endl;
                    string type;
                    cin>>type;
                    fstream file_write;
                    file_write.open("index.txt", ios::app);
                    file_write<<"Filename: "<<en->d_name<<endl<<"Type: "<<type<<endl;
                    string name = en->d_name;

                    fstream file_text;
                    file_text.open(directory + name, ios::in);
                    
                    while (!file_text.eof())
                    {
                        string line;
                        getline(file_text, line);
                        if (line.substr(0, 5) == "Title")
                            file_write<<line<<endl;

                        if (line.substr(0, 6) == "Author")
                        {
                            file_write<<line<<endl;
                            break;
                        }
                    }
                    file_write.close();
                    file_text.close();
                }
                file.close();
            }
        }
        closedir(dr); //close all directory
    }

    return directory;
}

//function to display all the available files which are named in "index.txt"
void display_files()
{
    fstream file_display;
    file_display.open("index.txt", ios::in);
    
    while (!file_display.eof())
    {
        string line;
        cout<<endl;
        for (int i = 0; i < 4; i++)
        {
            getline(file_display, line);
            cout<<line<<endl;
        }
    }
    file_display.close();
}

//function to search for a book in the directory via its title or its author name
void search_files(string directory)
{
    while (1)
    {
        int count = 0;
        vector <string> book;
        while (1)
        {
            cout<<"Enter 1 to search a book by its Title"<<endl<<"Enter 2 to search a book by its Author's Name"<<endl;
            int searcher;
            cin>>searcher;
            int flag = 0;

            if (searcher == 1)
            {
                cout<<"Enter the name of the title of the book"<<endl;
                string name;
                getline(cin, name);
                getline(cin, name);

                fstream file;
                file.open("index.txt", ios::in);
                while(!file.eof())
                {
                    string filename, type, title, author;
                    getline(file, filename);
                    getline(file, type);
                    getline(file, title);
                    getline(file, author);
                    
                    transform(title.begin(), title.end(), title.begin(), ::tolower); 
                    transform(name.begin(), name.end(), name.begin(), ::tolower); 
                    if (title.find(name) != string::npos)
                    {
                        count++;
                        cout<<endl<<"Book Found"<<endl<<"["<<count<<"]"<<endl;
                        cout<<filename<<endl;
                        cout<<type<<endl;
                        cout<<title<<endl;
                        cout<<author<<endl;
                        book.push_back(filename);
                        flag = 1;
                    }
                }
                if (flag == 0)
                    cout<<"No book found"<<endl;
                break;
            }

            else if (searcher == 2)
            {
                cout<<"Enter the name of the author of the book"<<endl;
                string name;
                getline(cin, name);
                getline(cin, name);

                fstream file;
                file.open("index.txt", ios::in);
                while(!file.eof())
                {
                    string filename, type, title, author;
                    getline(file, filename);
                    getline(file, type);
                    getline(file, title);
                    getline(file, author);

                    transform(author.begin(), author.end(), author.begin(), ::tolower); 
                    transform(name.begin(), name.end(), name.begin(), ::tolower); 
                    if (author.find(name) != string::npos)
                    {
                        count++;
                        cout<<endl<<"Book Found"<<endl<<"["<<count<<"]"<<endl;
                        cout<<filename<<endl;
                        cout<<type<<endl;
                        cout<<title<<endl;
                        cout<<author<<endl;
                        book.push_back(filename);
                        flag = 1;
                    }
                }
                
                if (flag == 0)
                    cout<<"No book found"<<endl;
                break;
            }

            else
            {
                cout<<"Wrong number input"<<endl;
                continue;
            }
        }
        cout<<endl<<"Number of books found: "<<count<<endl;
        if (book.size() != 0)
        {
            int select;
            while(1)
            {
                cout<<"Enter 1 to read the book. Enter 2 to perform analytics tasks on it."<<endl;
                cin>>select;

                if (select == 1)
                {
                    print_book(book, directory);
                    cout<<endl<<"Book Finshed"<<endl;
                    break;
                }

                else if (select == 2)
                {
                    analytics(book, directory);
                    break;
                }

                else
                    cout<<"Wrong number entered. Enter again"<<endl;
            }
        }

        cout<<"Enter 1 to search again. Enter anything else to exit"<<endl;
        int exiter;
        cin>>exiter;
        if (exiter != 1)
            break;
    }
}

//function that prints a book to the console
void print_book(vector <string> book, string directory)
{
    string filename = get_filename(book, directory);
    if (filename == "invalid")
        return;

    fstream file;
    file.open(filename, ios::in);

    while (!file.eof())
    {
        string line;
        for (int i = 0; i < 50; i++)
        {
            getline(file, line);
            cout<<line<<endl;
        }

        int flag; //0 for next, 1 for exit
        while (1)
        {
            cout<<endl<<endl<<"Press 'N' to go to the next page and 'E' to exit"<<endl;
            char parser;
            cin>>parser;

            if (parser == 'n' || parser == 'N')
            {
                flag = 0;
                break;
            }
            else if (parser == 'e' || parser == 'E')
            {
                flag = 1;
                break;
            }
            else
                cout<<"Wrong letter entered"<<endl;   
        }

        if (flag == 0)
            continue;
        else
            break;
    }
}

//function that performs different analytics on the Books (Play and Novel). Can be extended to other genres as well
void analytics(vector <string> book, string directory)
{
    string filename = get_filename(book, directory);
    if (filename == "invalid")
        return;

    string type;

    fstream file;
    file.open("index.txt", ios::in);
    while(!file.eof())
    {
        string file_name, type_, title, author;
        getline(file, file_name);
        getline(file, type_);
        getline(file, title);
        getline(file, author);
        if (hasEnding(filename, file_name.substr(10, file_name.length()-10)))
        {
            type = type_.substr(6, type_.length()-6);
            break;
        }
    }
    if (type == "Novel" || type == "novel")
    {
        Novel parser(filename); //object for class Novel
        cout<<"Enter the input word"<<endl;
        string input = parser.set_input();
        while(1)
        {
            int para_or_chapter = parser.para_or_chap();
            if (para_or_chapter != 1 && para_or_chapter != 2)
            {
                cout<<"Wrong Input";
                continue;
            }
            parser.counter();
            break;
        }
    }

    else if (type == "Play" || type == "play")
    {
        Play parser(filename); //object for class Play
        cout<<"Enter the Input Character"<<endl;
        string input = parser.set_input();

        parser.reader();
        parser.counter();
    }
}

//function that gives the complete filename of the book chosen
string get_filename(vector <string> book, string directory)
{
    cout<<endl<<"Enter the serial number of the book that you want to get displayed (The number displayed in the square bracketrs)"<<endl;
    int serial;
    cin>>serial;

    //if the serial number entered is invalid
    if (serial > book.size() || serial <= 0)
    {
        cout<<"Invalid serial number"<<endl;
        return "invalid";
    }

    return (directory + book[serial-1].substr(10, book[serial-1].length()-10));
}