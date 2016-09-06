#include "Storage.hpp"
#include "Path.hpp"
#include <iostream>
#include <fstream>

Storage::Storage(){
	m_dirty = false;
	readFromFile();
}

bool Storage::readFromFile(void) {
	
	std::ifstream fileM;
	fileM.open(Path::meetingPath);
	std::ifstream fileU;
	fileU.open(Path::userPath);
	if (!fileU || !fileM) {return false;}  //
	std::string get;
	std::string data[4]; 
	std::string name, pass, email, phone;
	while(std::getline(fileU, get)) {
		int count = 0;


		for (int i = 0; i < get.length(); i++) {
			if (get[i] == '"' && get[i  + 1] != ',') {
				int j = i;
				if (count < 3) {
					while (get[i] != ',') i++;
					data[count++] = get.substr(j + 1, i -2 - j);
				} else {
					i++;
					while (get[i] != '"') i++;
					data[count++] = get.substr(j + 1, i -1 - j);
					
				}
				
			}
		}

		User temp(data[0], data[1], data[2], data[3]);

		m_userList.push_back(temp);
	}

		std::vector<std::string> parts;

	while(std::getline(fileM, get)) {
		int i = 0, j = 0;
		int partStart, partEnd;
		while (get[i] != ',') i++;
		data[0] = get.substr(j + 1, i - 2 - j);// name in

		i++;
		partStart = i; // a' " '
		i++;

		while (get[i] != ',') i++;
		partEnd = i - 1; // a' " '
		j = i + 1;
		i+=2;
		while (get[i] != ',') i++;

		data[1] = get.substr(j + 1, i - 2 - j);// begin in

		i++;
		j = i;
		while (get[i] != ',') i++;
		data[2] = get.substr(j + 1, i - 2 - j);// end in

		i++;
		j = i;
		while(get[i] != '"') i++;
		data[3] = get.substr(j + 1, i - 2 - j);// title in
		data[3].pop_back();


		for (int i = partStart + 1; i < partEnd;) {
			int j;

			std::string a;
			if (get[i - 1] == '&' || get[i - 1] == '"') {
				j = i;
			}

			while (get[i] != '&' && get[i] != '"' ) {
				i++;}
			a = get.substr(j, i - j);
			i++;
			parts.push_back(a);
		}
		
			Meeting temp2(data[0], parts, Date::stringToDate(data[1]), Date::stringToDate(data[2]), data[3]);
			m_meetingList.push_back(temp2);
			parts.clear();
		
	}




		fileM.close();
		fileU.close();
		return true;

}

bool Storage::writeToFile(void) {
	if (m_dirty) {
		std::ofstream fileM;
		fileM.open(Path::meetingPath, std::ios::trunc);
		std::ofstream fileU;
		fileU.open(Path::userPath, std::ios::trunc);
		std::list<User>::iterator U;
		std::list<Meeting>::iterator M;
		if (!fileU || !fileM) {return false;}  //
		for (U = m_userList.begin(); U != m_userList.end();U++) {
			fileU << '"' << (*U).getName() << '"' << ',' 
				  << '"' << (*U).getPassword() << '"' << ','
				  << '"' << (*U).getEmail() << '"' << ','
				  << '"' << (*U).getPhone() << '"' << std::endl;
		}

		for (M = m_meetingList.begin(); M != m_meetingList.end(); M++) {
			fileM << '"' << (*M).getSponsor() << '"' << ',' << '"';
			std::vector<std::string> w = (*M).getParticipator();
			std::vector<std::string>::iterator e = w.begin();

			fileM << (*e);
			e++;
			for (; e < w.end(); e++) {
				fileM << '&' << (*e);
			}
			Date start, end;
			start = (*M).getStartDate();
			end = (*M).getEndDate();
			fileM << '"' << ',';
			fileM << '"' << Date::dateToString(start) << '"' << ','
				  << '"' << Date::dateToString(end) << '"' << ','
				  << '"' << (*M).getTitle() << '"' << std::endl;
		}
	fileM.close();
	fileU.close();
		m_dirty = false;
		return true;
	} else {
		return false;
	}
	

	
		
}

std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == NULL) {
		m_instance = std::shared_ptr<Storage>(new Storage());
	}
	return m_instance;
}

Storage::~Storage(){
	sync();
}

void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
	
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
	std::list<User> back;
	for(auto it = m_userList.begin(); it != m_userList.end(); it++) {
		if (filter(*it)) {
			back.push_back(*it);
		}
	}
	return back;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
			std::function<void(User &)> switcher) {
			
			int counter = 0;
			for (auto it = m_userList.begin(); it != m_userList.end(); it++) {
				if (filter(*it)) {
					switcher(*it);
					counter++;
					m_dirty = true;
				}
			}
			
			return counter;
}

int Storage::deleteUser(std::function<bool(const User &)>filter) {
			
			int counter = 0;
			for (auto it = m_userList.begin(); it != m_userList.end(); it++) {
				if (filter(*it)) {
					
					it = m_userList.erase(it);
					counter++;
					m_dirty = true;
				}
			}
			
			return counter;

}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
	sync();
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)>filter) const {
	std::list<Meeting> back;
	for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
		if (filter(*i)) {
			back.push_back(*i);
		}
	}
	return back;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
			   std::function<void(Meeting &)> switcher) {
			int counter = 0;
			for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
				if (filter(*i)) {
					switcher(*i);
					counter++;
					m_dirty = true;
				}
			}
			
			return counter;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
			int counter = 0;
			for (auto it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
				if (filter(*it)) {
					it = m_meetingList.erase(it);
					it--;
					counter++;
					m_dirty = true;
				}
			}
			
			return counter;
}

bool Storage::sync(void) {
	if (m_dirty = true) {
		if (writeToFile()) {
		m_dirty = false;}
		else{}
	}
	
}

std::shared_ptr<Storage> Storage::m_instance = NULL;