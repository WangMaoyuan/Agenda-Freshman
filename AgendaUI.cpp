#include "AgendaUI.hpp"
#include <iomanip>
using std::cout;
using std::endl;
using std::cin;
using std::string;

const string ops[15] = {"l", "r", "q", "o",
    "dc", "lu", "cm", "la", "las", "lap", "qm", "qt"
, "dm", "da", "printM"};
string operation;
    AgendaUI::AgendaUI() { startAgenda();}
    void AgendaUI::OperationLoop(void) {

        string op = "o";
        while (op != "q") {
            cout << "----------------------- Agenda -----------------------" << endl
             << "Action : " << endl 
             << "l\t- Log in agenda by user name and password" << endl
             << "r\t- Register an Agenda account" << endl
             << "q\t- Quit Agenda" << endl
             << "------------------------------------------------------";
            cout << endl << "Agenda : ~$ ";
            op = getOperation();

            if (op == "q") {
                quitAgenda();
            } else if (op == "BAD") {
                cout << "Unexpected input, Please try again later\n";
            } else if (op == "l" || op == "r"){
                if(executeOperation(op)) cout << "Command processed\n";
                else cout << "Unexpected error, try again\n";
            } else {
                cout << "You need to login your account for futher operation\n";
            }
        }
        
    }
// function below are all private
    /**
     * constructor
     */
    void AgendaUI::startAgenda(void) { // 
    }

    /**
     * catch user's operation
     * @return the operation
     */
    std::string AgendaUI::getOperation() {

        cin >> operation;
        
        bool flag = false;
        for (auto i : ops) {
            if (i == operation) {
                flag = true;
                break;
            }
        }
        if (flag) {
            cout << "Processing your command.....\n";
            return operation;
        }
        else {
            return "BAD";
        } 
    }

    /**
     * execute the operation
     * @return if the operationloop continue
     */
    bool AgendaUI::executeOperation(std::string t_operation) {
        int casen;
        for (casen = 0; casen < 15; casen++) {
            if (ops[casen] == t_operation) {
                break;
            }
        }
        switch (casen) {
            case 0 : 
                        userLogIn();
                        return true;
                        break;
            case 3 : 
                        userLogOut();
                        return true;
                        break;
            case 1 : 
                        userRegister();
                        return true;
                        break;
            case 5 : 
                        listAllUsers();
                        return true;
                        break;
            case 4 : 
                        deleteUser();
                        return true;
                        break;
            case 6 : 
                        createMeeting();
                        return true;
                        break;
            case 7 : 
                        listAllMeetings();
                        return true;
                        break;
            case 8 : 
                        listAllSponsorMeetings();
                        return true;
                        break;
            case 9 : 
                        listAllParticipateMeetings();
                        return true;
                        break;
            case 13 : 
                        deleteAllMeetings();
                        return true;
                        break;
            case 12 : 
                        deleteMeetingByTitle();
                        return true;
                        break;
            case 11 : 
                        queryMeetingByTimeInterval();
                        return true;
                        break;
            case 10 : 
                        queryMeetingByTitle();
                        return true;
                        break;
            /*case printM : 
                        printMeetings();
                        return true;
                        break;*/
                default: return false;
        }
    }

    void AgendaUI::userLogIn(void) {
        string command = "l";
        char space;
        cout << "[log in] [user name] [password]" << endl
             << "[log in] ";
        cin >> m_userName;
        cin >> m_userPassword;

        if (m_userName.length() != 0 && m_userPassword.length() != 0
         && m_agendaService.userLogIn(m_userName, m_userPassword)) {
            cout << "[log in] succeed" << endl;
            while (command != "o") {

            
            cout << "----------------------- Agenda -----------------------" << endl
                 << "Action : " << endl
                 << "dc\t- delete Agenda account" << endl
                 << "o\t- log out Agenda" << endl
                 << "lu\t- list all Agenda users" << endl
                 << "cm\t- create a meeting" << endl
                 << "la\t- list all meetings" << endl
                 << "las\t- list all sponsor meetings" << endl
                 << "lap\t- list all participate meetings" << endl
                 << "qm\t- query meeting by title" << endl
                 << "qt\t- query meeting by time interval" << endl
                 << "dm\t- delete meeting by title" << endl
                 << "da\t- delete all meetings" << endl
                 << "----------------------------------------------------" << endl
                 << "Agenda@" << m_userName <<" : # " ;


             command = getOperation();

            if(command == "l" || command == "r") {
                cout << "You can login or sign in after log out\n";
            } else if (command == "BAD") {
                cout << "Unexpected input, Please try again later\n";
            } else {
                if (command == "dc") {
                    cout << "This command would deletet your account,continue? y/n : ";
                    char op;
                    cin >> op;
                    if (op == 'y') {
                        executeOperation(command);
                        command = 'o';
                    } 
                } 
                else executeOperation(command);
                
            }
        }

        } else {
            cout << "[error] log in fail!\n";

        }

    }

    /**
     * user regist
     */
    void AgendaUI::userRegister(void) {
        string name, password, email, phone;
        cout << "[register] [user name] [password] [email] [phone]" << endl << "[register] ";
        cin >> name;
        cin >> password;
        cin >> email;
        cin >> phone;

        if(name.length() != 0 && password.length() != 0 && email.length() != 0
            && phone.length() != 0
            && m_agendaService.userRegister(name, password, email, phone)) {
            cout << "[register] succeed!\n"; 
        } else {
            cout << "[register] fail!\n"; 
        }

    }

    /**
     * quit the Agenda
     */
    void AgendaUI::quitAgenda(void) {
        cout << "Thank you for using, bye." << endl;
    }

    /**
     * user logout
     */
    void AgendaUI::userLogOut(void) {
        cout << "[log out] succeed!\n";
        
    }

    /**
     * delete a user from storage
     */
    void AgendaUI::deleteUser(void) {
        if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
            cout << "[delete Agenda account] succeed!\n";
        }
    }

    /**
     * list all users from storage
     */
    void AgendaUI::listAllUsers(void) {
        cout << "[list all users]" << endl
             << endl << setiosflags(std::ios::left) << std::setw(15) << "name"
             << setiosflags(std::ios::left) << std::setw(15) <<"email" 
             << setiosflags(std::ios::left) << std::setw(15) << "phone" << endl;
        std::list<User> back = m_agendaService.listAllUsers();
        for (auto i : back) {


            cout.fill(' ');
            cout << setiosflags(std::ios::left) << std::setw(15) << i.getName() 
                 << setiosflags(std::ios::left) << std::setw(15) << i.getEmail() 
                 << setiosflags(std::ios::left) << std::setw(15) << i.getPhone() << endl;
        } 
    }

    /**
     * user create a meeting with someone else
     */
    void AgendaUI::createMeeting(void) {
        cout << "[create meeting] " << "[the number of participators]" << endl;
        cout << "[create meeting] " ;
        std::string t;
        cin >> t;
        int number = 0;
        bool flag = true;
        for (int q = 0; q < t.length(); q++) {
            if (t[q] < '0' || t[q] > '9') {
                flag = false;
                break;}
        }
        if (flag) {
            for (int q = 0; q < t.length(); q++) {
                number += (t[q] - 48);
            if (q != t.length() - 1) {
                number *= 10;
                }
        }
        std::vector<string> NAME;

        for (int i = 1; i <= number; i++) {
            cout << "[create meeting] " << "[please enter the participator " << i << " ]" << endl;
            cout << "[create meeting] ";
            string name;
            cin >> name;
            NAME.push_back(name);
        }
        cout << "[create meeting] " << "[title] [start time(yyyy-mm-dd/hh:mm)]"
             << " [end time(yyyy-mm-dd/hh:mm)]" << endl << "[create meeting] ";

        string title;
        string linkStart;
        string linkEnd;
        cin >> title >> linkStart >> linkEnd;

        bool flag = true;

        for (auto i : NAME) {
            if (i == m_userName) {
                flag = false;
                break;
                }
        }


        if (flag && title.length() != 0 && linkStart.length() != 0
            && linkEnd.length() != 0 && NAME.size() > 0
            && m_agendaService.createMeeting(m_userName, title, linkStart
            , linkEnd, NAME)) {
            cout << "[create meeting] succeed!\n";
        } else {
            cout << "[create meeting] error!\n";
        }}
        else {
            cout << "[create meeting] error!\n";
        }
    }

    /**
     * list all meetings from storage
     */
    void AgendaUI::listAllMeetings(void) {
        cout << "[list all meetings]" << endl
             << endl << setiosflags(std::ios::left) << std::setw(18) << "title"
             << setiosflags(std::ios::left) << std::setw(18) <<"sponsor" 
             << setiosflags(std::ios::left) << std::setw(18) << "start time" 
             << setiosflags(std::ios::left) << std::setw(18) << "end time" 
             << setiosflags(std::ios::left) << std::setw(18) << "participators" << endl;


        std::list<Meeting> back = m_agendaService.listAllMeetings(m_userName);
        printMeetings(back);
    }

    /**
     * list all meetings that this user sponsored
     */
    void AgendaUI::listAllSponsorMeetings(void) {
        cout << "[list all sponsor meetings]" << endl
             << endl << setiosflags(std::ios::left) << std::setw(18) << "title"
             << setiosflags(std::ios::left) << std::setw(18) <<"sponsor" 
             << setiosflags(std::ios::left) << std::setw(18) << "start time" 
             << setiosflags(std::ios::left) << std::setw(18) << "end time" 
             << setiosflags(std::ios::left) << std::setw(18) << "participators" << endl;


        std::list<Meeting> back = m_agendaService.listAllSponsorMeetings(m_userName);
        printMeetings(back);
    }

    /**
     * list all meetings that this user take part in
     */


// empty check!




    void AgendaUI::listAllParticipateMeetings(void) {
        cout << "[list all participator meetings]" << endl
             << endl << setiosflags(std::ios::left) << std::setw(18) << "title"
             << setiosflags(std::ios::left) << std::setw(18) <<"sponsor" 
             << setiosflags(std::ios::left) << std::setw(18) << "start time" 
             << setiosflags(std::ios::left) << std::setw(18) << "end time" 
             << setiosflags(std::ios::left) << std::setw(18) << "participators" << endl;


        std::list<Meeting> back = m_agendaService.listAllParticipateMeetings(m_userName);
        printMeetings(back);
    }

    /**
     * search meetings by title from storage
     */
    void AgendaUI::queryMeetingByTitle(void) {
        cout << "[query meeting] " << "[title]:" << endl << "[query meeting] ";

        string title;
        cin >> title;
        if (title.length() != 0) {
        std::list<Meeting> Q = m_agendaService.meetingQuery(m_userName, title);
        cout << endl << setiosflags(std::ios::left) << std::setw(18) << "title"
             << setiosflags(std::ios::left) << std::setw(18) <<"sponsor" 
             << setiosflags(std::ios::left) << std::setw(18) << "start time" 
             << setiosflags(std::ios::left) << std::setw(18) << "end time" 
             << setiosflags(std::ios::left) << std::setw(18) << "participators" << endl;
        printMeetings(Q);}
        else cout << "No valid input" <<endl;
    }

    /**
     * search meetings by timeinterval from storage
     */
    void AgendaUI::queryMeetingByTimeInterval(void){
        cout << "[query meeting] " << "[start time(yyyy-mm-dd/hh:mm)]"
             << " [end time(yyyy-mm-dd/hh:mm)]" << endl << "[query meeting] ";
        string start, end;
        cin >> start >> end;
        if (start.length() == 16 && end.length() == 16 ) {
        std::list<Meeting> Q = m_agendaService.meetingQuery(m_userName, start, end);
        cout << endl << setiosflags(std::ios::left) << std::setw(18) << "title"
             << setiosflags(std::ios::left) << std::setw(18) <<"sponsor" 
             << setiosflags(std::ios::left) << std::setw(18) << "start time" 
             << setiosflags(std::ios::left) << std::setw(18) << "end time" 
             << setiosflags(std::ios::left) << std::setw(18) << "participators" << endl;
        printMeetings(Q);}
        else cout << "No valid input" <<endl;
    }

    /**
     * delete meetings by title from storage
     */
    void AgendaUI::deleteMeetingByTitle(void) {
        cout << endl;
        cout << "[delete meeting] [title]" << endl << "[delete meeting] ";

        string title;
        cin >> title;

        cout << "[delete meeting by title] ";

        if (m_agendaService.deleteMeeting(m_userName, title)) {
            cout << "succeed!" << endl;
        } else {
            cout << "fail!" << endl;
        }
    }

    /**
     * delete all meetings that this user sponsored
     */
    void AgendaUI::deleteAllMeetings(void) { 
        cout << endl << "[delete all meetings] ";
        cout << (m_agendaService.deleteAllMeetings(m_userName))?"succeed!\n" : "fail!\n";
    }

    /**
     * show the meetings in the screen
     */
    void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
        for (auto i : t_meetings) {

            cout.fill(' ');
            cout << setiosflags(std::ios::left) << std::setw(18) << i.getTitle() 
                 << setiosflags(std::ios::left) << std::setw(18) << i.getSponsor() 
                 << setiosflags(std::ios::left) << std::setw(18) <<  Date::dateToString(i.getStartDate())
                 << setiosflags(std::ios::left) << std::setw(18) << Date::dateToString(i.getEndDate());

            std::vector<string> part = i.getParticipator();
            cout << part[0];
            for (auto  it = part.begin() + 1; it != part.end(); it++) {
                cout << ',' << (*it);
            }
            cout << endl;
        }
    }
    // dates
