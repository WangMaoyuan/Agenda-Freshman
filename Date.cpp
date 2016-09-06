#include "Date.hpp"
#include  <iostream>
#include <string>
Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_minute = 0;
	m_hour = 0;
}

Date::Date(int t_year, int t_month, int t_day,
	   int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(std::string dateString) {
	Date date = stringToDate(dateString);
	m_year = date.getYear();
	m_month = date.getMonth();
	m_day = date.getDay();
	m_hour = date.getHour();
	m_minute = date.getMinute();
}

int Date::getYear(void) const {
	return m_year;
}
void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}
void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}
void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}
void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}
void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {

	if (t_date.getYear() > 9999 || t_date.getYear() < 1000) {
	    return false;	
	}
	if (t_date.getMonth() > 12 || t_date.getMonth() < 1) {
            return false;
        }
	if (t_date.getHour() > 23 || t_date.getHour() < 0) {
            return false;
        }
	if (t_date.getMinute() > 59 || t_date.getMinute() < 0) {
            return false;
        }
	int day = t_date.getDay();
	int month = t_date.getMonth();
	int year = t_date.getYear();
	if (day > 31 || day < 1) {return false;}
	
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) {return false;}
	}

	if (month == 2) {
		if (day > 29) {
		  return false;
		}
		if ((year%100 == 0 && year%400 != 0) || (year%4 != 0)) {
		  if (day > 28) {
		    return false;
			}
		}
	}


	return true;
}

Date Date::stringToDate(const std::string t_dateString) {
	const std::string back = t_dateString;
	Date data;
	if (back.size() != 16) {
	return data;
	}
	if(back[4] != '-' || back[7] != '-' ||
	   back[10] != '/' || back[13] != ':') {
	return data;
	}
	for (int i = 0; i < 16; i++) {
	  if (i != 4 && i != 7 && i != 10 && i!= 13) {
		if (t_dateString[i] > '9' || t_dateString[i] < '0') {
		  return data;
		}
	  }
	}
	int Temp = 0;
	Temp += (back[15]-48) + 10 * (back[14] - 48);
	data.setMinute(Temp);
	Temp = 0;
	Temp += (back[12]-48) + 10 * (back[11] - 48);
 	data.setHour(Temp);
	Temp = 0;
	Temp += (back[9]-48) + 10 * (back[8] - 48);
	data.setDay(Temp);
	Temp = 0;
        Temp += (back[6]-48) + 10 * (back[5] - 48);
        data.setMonth(Temp);

	Temp = 0;
	for (int i = 0; i < 4; i++) {
	  Temp += (back[i] - 48);
	  if (i!= 3) Temp *= 10;
	}
	data.setYear(Temp);

	return data;



}
std::string Date::dateToString(Date t_date) {
	if (isValid(t_date)) {
    char temp;
	std::string b = "";
      
	int year, month, day, hour, minute;
      
	year = t_date.getYear();
	month = t_date.getMonth();
	day = t_date.getDay();
	hour = t_date.getHour();
	minute = t_date.getMinute();
	

	temp = year / 1000;
	temp += 48;
	b += temp;
	temp = year / 100 % 10;
	temp += 48;
	b += temp;
	temp = year / 10 % 10;
temp += 48;
	b += temp;
	temp = year % 10;
	 temp += 48;
	b += temp;

	temp = '-';
	b += temp;

	temp = month / 10;
temp += 48;
	b += temp;
	temp = month % 10;
temp += 48;
	b += temp;

	temp = '-';
	b += temp;

	temp = day / 10;
temp += 48;
	b += temp;
	temp = day % 10;
temp += 48;
	b += temp;

	temp = '/';
	b += temp;

	temp = hour / 10;
temp += 48;
	b += temp;
	temp = hour % 10;
temp += 48;
	b += temp;

	temp = ':';
	b += temp;

	temp = minute / 10;
temp += 48;
	b += temp;
	temp = minute % 10;
temp += 48;
	b += temp;
	return b;}
	else 
	{
		return "0000-00-00/00:00";
	}


}

Date& Date::operator= (const Date& t_date) {
	m_year = t_date.getYear();
	m_month = t_date.getMonth();
	m_day = t_date.getDay();
	m_hour = t_date.getHour();
	m_minute = t_date.getMinute();
}

bool Date::operator== (const Date &t_date) const {
	return (m_year == t_date.getYear() && m_month == t_date.getMonth()
		&& m_day == t_date.getDay() && m_hour == t_date.getHour() &&
		 m_minute == t_date.getMinute()) ? true : false;
}

bool Date::operator>(const Date &t_date) const {
	return dateToString(*this) > dateToString(t_date);
}
bool Date::operator<(const Date &t_date) const {
	return dateToString(*this) < dateToString(t_date);
}
bool Date::operator<=(const Date &t_date) const {
	if (!((*this) > t_date)) return true;
	else return false;
}
bool Date::operator>=(const Date &t_date) const {
	if (!((*this) < t_date)) return true;
        else return false;

}
