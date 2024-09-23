#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>

using namespace std;
namespace fs = filesystem;

class FileHandler{

    public :

        void help(){
            cout<<"Usage : .test_shell.exe <operation> <source> <destination>"<<endl;
            cout << "Operations:" << endl;
            cout << "  copy    - Copy a file or directory" << endl;
            cout << "  move    - Move a file or directory" << endl;
            cout << "  rename  - Rename a file or directory" << endl;
            cout << "  dcreate  - Create a directory" << endl;
            cout << "  fcreate  - Create a file" << endl;
            cout << "  perm  - See the permissions for a file" << endl;
            cout << "  exit  - Exit the CustomShell" << endl;
            cout << endl;
            cout << "Options:" << endl;
            cout << "  --help, -h    - Show this help message" << endl;
        }

        string strip_quotes(const string& path){

            size_t first = path.find_first_not_of(' ');
            size_t last = path.find_last_not_of(' ');

            if(first==string::npos || last==string::npos){
                return "";
            }

            string trimmed_path = path.substr(first, (last-first+1));

            if(path.front()=='"' && path.back()=='"'){
                return trimmed_path.substr(1, path.size() - 2);
            }
            else{
                return trimmed_path;
            }
        }

        void create_directory(const fs::path& path){
            try{
                fs::create_directory(path);
            }
            catch(const fs::filesystem_error& e){
                cerr<<"Error creating directory : "<<e.what()<<endl;
            }
        }

        void create_file(const fs::path& path){
            if(!fs::exists(path)){
                ofstream file(path);
            }
            else{
                cerr<<"A file with this name already exists"<<endl;
            }
        }

        void copy_file(const fs::path& from, const fs::path& to){
            try{
                fs::copy(from, to, fs::copy_options::overwrite_existing);
            }
            catch(const fs::filesystem_error& e){
                cerr<<"Error copying the file : "<<e.what()<<endl;
            }
        }

        void move_file(const fs::path& from, const fs::path& to){
            try{
                fs::rename(from, to);
            }
            catch(const fs::filesystem_error& e){
                cerr<<"Error moving file : "<<e.what()<<endl;
            }
        }

        void rename_file(const fs::path& old_name, const fs::path& new_name){
            try{
                fs::rename(old_name, new_name);
            }
            catch(const fs::filesystem_error& e){
                cerr<<"Error renaming file : "<<e.what()<<endl;
            }
        }

        void display_permissions(const fs::path& path){
            
            try{

                fs::perms permission = fs::status(path).permissions();
                cout<<"Permission : "<<path<<endl;

                if((permission & fs::perms::owner_read) != fs::perms::none){
                    cout<<"r";
                }
                else{
                    cout<<"-";
                }

                if((permission & fs::perms::owner_write) != fs::perms::none){
                    cout<<"w";
                }
                else{
                    cout<<"-";
                }

                if((permission & fs::perms::owner_exec) != fs::perms::none){
                    cout<<"x";
                }
                else{
                    cout<<"-";
                }
                if((permission & fs::perms::group_read) != fs::perms::none){
                    cout<<"r";
                }
                else{
                    cout<<"-";
                }

                if((permission & fs::perms::group_write) != fs::perms::none){
                    cout<<"w";
                }
                else{
                    cout<<"-";
                }

                if((permission & fs::perms::group_exec) != fs::perms::none){
                    cout<<"x";
                }
                else{
                    cout<<"-";
                }
                if((permission & fs::perms::others_read) != fs::perms::none){
                    cout<<"r";
                }
                else{
                    cout<<"-";
                }

                if((permission & fs::perms::others_write) != fs::perms::none){
                    cout<<"w";
                }
                else{
                    cout<<"-";
                }
                if((permission & fs::perms::others_exec) != fs::perms::none){
                    cout<<"x";
                }
                else{
                    cout<<"-";
                }    
                cout<<endl;      
            }
            catch(const fs::filesystem_error& e){
                cout<<"Error displaying permission : "<<e.what()<<endl;
            }
        }

        void change_directory(const fs::path& path){
            try{
                if(path==".."){
                    fs::current_path(fs::current_path().parent_path());
                }
                else{
                    fs::current_path(path);
                }
            }
            catch(const fs::filesystem_error& e){
                cerr<<e.what()<<endl;
            }
        }

        void compile_file(const string& source_file, const string& output_file){

            string command = "g++ " + source_file + " -o " + output_file;
            int result = system(command.c_str());

            if(result==0){
                cout<<output_file<<endl;
            }
            else{
                cerr<<"Compilation failed for "<<source_file<<endl;
            }
        }

        void run_executable(const string& output_file){

            string command = output_file;
            int result = system(command.c_str());

            if(result!=0){
                cerr<<"Error running executable : "<<output_file<<endl;
            }
        }
};

class CustomShell{

    FileHandler fh;

    public:

        CustomShell(FileHandler fh){
            this->fh = fh;
        }

        void display_starting_message(){
            cout<<"Welcome to Custom Shell"<<endl;
            cout<<"For help, type --help or -h"<<endl;
            cout<<endl;
        }

        void run_shell(){

            string line, command, arg1, arg2;
            
            while(true){

                cout<<fs::current_path()<<">> ";
                getline(cin, line);

                if(line.empty()){
                    continue;
                }

                istringstream iss(line);
                iss>>command;

                getline(iss, arg1, '"');
                getline(iss, arg1, '"');
                getline(iss, arg2, '"');
                getline(iss, arg2, '"');

                arg1 = fh.strip_quotes(arg1);
                //cout<<arg1<<endl;
                arg2 = fh.strip_quotes(arg2);
                //cout<<arg2<<endl;
                
                if(command=="--help" || command=="-h"){
                    fh.help();
                }
                else if(command=="copy"){
                    fh.copy_file(arg1, arg2);
                }
                else if(command=="move"){
                    fh.move_file(arg1, arg2);
                }
                else if(command=="rename"){
                    fh.rename_file(arg1, arg2);
                }
                else if(command=="dcreate"){
                    fh.create_directory(arg1);
                }
                else if(command=="fcreate"){
                    fh.create_file(arg1);
                }
                else if(command=="perm"){
                    fh.display_permissions(arg1);
                }
                else if(command=="cd"){
                    fh.change_directory(arg1);
                }
                else if(command=="compile"){
                    fh.compile_file(arg1, arg2);
                }
                else if(command=="run"){
                    fh.run_executable(arg1);
                }
                else if(command=="exit"){
                    cout<<"Exiting....."<<endl;
                    break;
                }
            }
        }

};

int main(){

    FileHandler fh;
    CustomShell cs(fh);
    cs.display_starting_message();
    cs.run_shell();

    return 0;
}