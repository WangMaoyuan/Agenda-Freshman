#include "AgendaService.hpp"
using namespace std;
#include <vector>
#include <list>
#include <string>
    AgendaService::AgendaService() {
        startAgenda();
    }
        AgendaService::~AgendaService() {
            quitAgenda();
    }
    bool AgendaService::userLogIn(const std::string userName, const std::string password) {

        if (userName.length() == 0 && password.length() == 0) return false;

        std::list<User> U = m_storage->queryUser([userName, password] (const User& i) -> bool{
            if (i.getName() == userName) {
                if (i.getPassword() == password) return true;
                else return false;  
            } else {return false;}
            
        });

        if (U.size() == 0) {return false;}
        else return true;
    }


    bool AgendaService::userRegister(const std::string userName, const std::string password, const std::string email, const std::string phone) {
        std::list<User> U = m_storage->queryUser([userName] (const User& i) -> bool{
            if (i.getName() == userName) return true;
            else return false;
        });

        if (U.size() != 0) {return false;} // name overlap

        User temp(userName, password, email, phone);
        m_storage->createUser(temp);
        return true;

    }


    bool AgendaService::deleteUser(const std::string userName, const std::string password) {
        
        
        auto filter1 = [userName, password](const User &user) -> bool {
            return (user.getName() == userName && user.getPassword() == password);
        };

        if (m_storage->queryUser(filter1).size() <= 0) return false;

        auto func = [userName](const Meeting &i_meeting) -> bool {

        if (i_meeting.getSponsor() == userName) {
             return true;
        }

        std::vector<std::string> v = i_meeting.getParticipator();

        for (int each =  0; v.begin() + each < v.end(); each++) {
            if (v[each] == userName) {
                 return true;
            }
        }
        return false;

    };

        int a = m_storage->deleteUser([userName, password] (const User& i) -> bool{
            if (i.getName() == userName) {
                if (i.getPassword() == password) return true;
                else return false;
            }
            else return false;
        });

        int b = m_storage->deleteMeeting(func); // maybe no meeting


            if (a != 0) return true;
            else return false;
}
        // delete related meetings

    std::list<User> AgendaService::listAllUsers(void) const {
        std::list<User> back = m_storage->queryUser([](const User& i) -> bool {
            return true;
        });
        return back;
    }

    /**
     * create a meeting
     * @param userName the sponsor's userName
     * @param title the meeting's title
     * @param participator the meeting's participator
     * @param startData the meeting's start date
     * @param endDate the meeting's end date
     * @return if success, true will be returned
     */
    bool AgendaService::createMeeting(const std::string userName, const std::string title,
     const std::string startDate, const std::string endDate, const std::vector<std::string> participator) {
        Date a1(startDate);
        Date a2(endDate);
    if (!Date::isValid(a1) || !Date::isValid(a2)) return false; // date

    if (a2 <= a1) return false; //valid date

    if (participator.size() == 0) return false;// have participators

    auto filtersuper = [userName] (const User& user)  -> bool {
        return userName == user.getName();
    };
    if (m_storage -> queryUser(filtersuper).size() == 0) {
        return false;
    }    // user registered

    for (int i = 0; i < participator.size(); i++) {
        for  (int j = i+1; j < participator.size(); j++) {
                if (participator[i] == participator[j]) return false;
        }
    }   // overlap participator

    for (auto each : participator) {
        if (each == userName) return false; // sponsor and participator are different
    }

    for (auto each : participator) {
        auto fil = [=](const User& t_user) -> bool {
            if (each == t_user.getName()) return true;
            return false;
        };
        std::list<User> temp1 = m_storage->queryUser(fil);
        if (temp1.size() == 0) return false;
    }

    // no man unregistered 


    auto funcc1 = [title](const Meeting &meet) -> bool {
        return title == meet.getTitle() ;
    };
        std::list<Meeting> smeet = m_storage->queryMeeting(funcc1);
        if (smeet.size() != 0) return false;


    // no same title
    std::list<Meeting> MetList = listAllMeetings(userName);
    for (auto each : MetList) {
        if (each.getStartDate() >= a2) continue;
        if (each.getEndDate() <= a1) continue;
        return false;
    } // sponsor have time

    for (auto eachS : participator) {
        std::list<Meeting> MetList = listAllMeetings(eachS);
        for (auto each : MetList) {
        if (each.getStartDate() >= a2) continue;
        if (each.getEndDate() <= a1) continue;
        return false;
        }
    }

    Meeting temp(userName, participator, a1, a2, title);
    m_storage->createMeeting(temp);
    return true;

    }

    std::list<Meeting> AgendaService::meetingQuery(const std::string userName, const std::string title) const {
        std::list<Meeting> back;

        back = m_storage->queryMeeting([userName, title](const Meeting&i)->bool{
            if (title == i.getTitle()) {
                if (userName == i.getSponsor()) {return true;}

                // working as participator
                std::vector<std::string> v = i.getParticipator();
                for (auto each : v) {
                if (each == userName) {
                     return true;
                    }
                }

                return false;
            } else {
                return false;
                }
        });
        return back;
    }

    std::list<Meeting> AgendaService::meetingQuery(const std::string userName, 
        const std::string startDate, const std::string endDate) const {
        std::list<Meeting> back;


    if (!Date::isValid(Date::stringToDate(startDate))) {
        return back;
    }
    if (!Date::isValid(Date::stringToDate(endDate))) {
        return back;
    }
    if (Date::stringToDate(startDate) > Date::stringToDate(endDate)) {
        return back;}


        back = m_storage->queryMeeting([startDate, endDate, userName](const Meeting&i)->bool{
            if (userName == i.getSponsor()) {
                Date st = Date::stringToDate(startDate);
                Date end = Date::stringToDate(endDate);
                if (i.getStartDate() > end || i.getEndDate() < st) {return false;}
                return true;
            }

                std::vector<std::string> v = i.getParticipator();
                if (i.isParticipator(userName)) {
                        Date st = Date::stringToDate(startDate);
                        Date end = Date::stringToDate(endDate);
                        if (i.getStartDate() > end || i.getEndDate() < st) {return false;}
                    return true;
            }
            return false;
        });
        return back;
}

    /**
     * list all meetings the user take part in
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const{
    std::list<Meeting> back;

    back = m_storage->queryMeeting([=] (const Meeting& i)->bool {
            if (userName == i.getSponsor()) {return true;}
            std::vector<std::string> t = i.getParticipator();
            for (int i = 0; i < t.size(); i++) {
                if (t[i] == userName) {
                   return true;
                }
            }
            return false;
        });
        return back;
}

    /**
     * list all meetings the user sponsor
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
            std::list<Meeting> back;

            back = m_storage->queryMeeting([=] (const Meeting& i)->bool {
            if ((userName == i.getSponsor())) {return true;}
            else return false;
        });
        return back;
}


    /**
     * list all meetings the user take part in and sponsor by other
     * @param userName user's username
     * @return a meeting list result
     */
    std::list<Meeting> AgendaService::listAllParticipateMeetings(
        const std::string userName) const{  
            std::list<Meeting> back;

            back = m_storage->queryMeeting([=] (const Meeting& i)->bool {
            std::vector<std::string> t = i.getParticipator();
            for (int i=0;i < t.size(); i++) {
                if (t[i] == userName) {
                     return true;
                }
            }
            return false;
        });
        return back;
}


    /**
     * delete a meeting by title and its sponsor
     * @param userName sponsor's username
     * @param title meeting's title
     * @return if success, true will be returned
     */
    bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {

        auto filtersuper = [userName] (const User& user)  ->bool {
                return userName == user.getName();
            };
            if (m_storage -> queryUser(filtersuper).size() <= 0) {
                return false;
            }
    int i = m_storage->deleteMeeting([=] (const Meeting& i) -> bool {
        if (i.getSponsor() == userName) {
          if (i.getTitle() == title) return true;
          else return false;
        }


    });
    if (i == 0) return false;
    else return true;
}

    /**
     * delete all meetings by sponsor
     * @param userName sponsor's username
     * @return if success, true will be returned
     */
    bool AgendaService::deleteAllMeetings(const std::string userName){

        auto filtersuper = [userName] (const User& user) ->bool  {
        return userName == user.getName();
    };
    if (m_storage -> queryUser(filtersuper).size() <= 0) {
        return false;
    }
    std::list<Meeting> my = listAllSponsorMeetings(userName);
    int i;
    if (my.size() != 0) {
        i = m_storage->deleteMeeting([my](const Meeting& i) -> bool {
        auto it = my.begin();
        bool flag = false;
        while (it != my.end()) {
            if (it->getTitle() == i.getTitle()) {
                flag = true;
                break;
            }
            it++;
        }
        return flag;
});
        if (i == 0) return false;
        else return true;
    } else return false;
}

    /**
     * start Agenda service and connect to storage
     */
    void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

    /**
     * quit Agenda service
     */
    void AgendaService::quitAgenda(void) {
}