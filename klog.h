#pragma once
#define GL_LOG_FILE "ArtProj.log"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <ctime>
using namespace std;
namespace {

static std::fstream g_File;
enum class log_level {DBG, WARN, ERR};
//static char _msg_[1024] = { 0 };
}


static bool start_klog() {
#if !defined(__GNUC__)
	FILE* file = nullptr; errno_t err = fopen_s(&file, GL_LOG_FILE, "w+");
#else
	FILE* file = fopen(GL_LOG_FILE, "w+");
#endif
	if (!file) {
		fprintf(stderr, "ERROR: could not open %s log file for writing\n", GL_LOG_FILE);
		return false;
	}
	time_t now = time(NULL);
	char date[64] = { 0 };
#if !defined(__GNUC__)
	err = ctime_s(date, 64, &now);
#else
	ctime_r(&now, date);
#endif
	fprintf(file, "%s @local time %s\n", GL_LOG_FILE, date);
	fprintf(stdout, "%s @local time %s\n", GL_LOG_FILE, date);
	fclose(file);
	return true;
}

static bool Klog(std::fstream& file, log_level lvl, const char* function, int line){

	file.open(GL_LOG_FILE, ios::app);
	if (file.fail() || file.bad()) {
		cerr << "ERROR: could not open " << GL_LOG_FILE << "%s for writing" << endl;
		return false;
	}

	switch (lvl)
	{
	case log_level::DBG:
		file << "[DBG] >>";
		break;
	case log_level::ERR:
		file << "[ERR] >>";
		break;
	case log_level::WARN:
		file << "[WRN] >>";
		break;
	default:
		break;
	}
	file << function << "(..):" << line << "\t\t";
#if _DEBUG
	cout << function << "(..):" << line << "\t\t";
#endif
	return true;
}

//usage: LOG_D("TEST" << 23 << obj.getMessage() << ex.getException() << "Another string")
#if !defined(DISABLE_LOG)
#define LOG_D(_X_)  {if(Klog(g_File, log_level::DBG, __FUNCTION__, __LINE__)){g_File << _X_ << endl; g_File.close();} cout << _X_ << endl;}
#define LOG_W(_X_)  {if(Klog(g_File, log_level::WARN, __FUNCTION__, __LINE__)){g_File << _X_ << endl;g_File.close();} cout << _X_ << endl;;}
#define LOG_E(_X_)  {if(Klog(g_File, log_level::ERR, __FUNCTION__, __LINE__)){g_File << _X_ << endl; g_File.close();} cerr << _X_ << endl;}
#else
#define LOG_D(_X_)
#define LOG_E(_X_)
#endif
