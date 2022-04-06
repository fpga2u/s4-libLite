#pragma once

#if defined _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4566 )
#pragma warning( disable : 4459 )
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#if defined _MSC_VER
#pragma warning( pop )
#endif



#include <iostream>
#include <filesystem>

#include <string>
#include <map>


namespace S4{


#define LOG_SIZE_MB (1024 * 1024)
class s4logger : std::enable_shared_from_this<s4logger>
{
public:
	struct cfg_t {
		bool enable_console = (bool)true;
		bool enable_file_all = (bool)false;
		bool enable_file_all_pure = (bool)true;
		bool enable_file_err = (bool)false;
		bool enable_file_err_pure = (bool)true;
		spdlog::level::level_enum level = (spdlog::level::level_enum)2;
		size_t max_file_size_MB = (size_t)9999;
		size_t max_files = (size_t)10;
		std::string save_path = (std::string)"./logs";
		std::string file_preamble = (std::string)"S4";
	};
public:
    typedef std::shared_ptr<s4logger> Ptr;

	s4logger();

	static Ptr pInstance() {
		static Ptr _g_pLogger;
		if (!_g_pLogger) {
			init_time_base();

			_g_pLogger = std::make_shared<s4logger>();
			_g_pLogger->init();
		}

		return _g_pLogger;
	}

    void init();
	void init(const s4logger::cfg_t&) noexcept;

	template<typename... Args>
	void err(const std::string& preamble, const char *fmt, const Args &... args)
	{
		std::string s = preamble + fmt;
		if (_file_all) {
			_file_all->error(s.c_str(), args...);
		}
		if (_file_all_pure) {
			_file_all_pure->error(s.c_str(), args...);
		}
		if (_file_err) {
			_file_err->error(s.c_str(), args...);
		}
		if (_file_err_pure	) {
			_file_err_pure->error(s.c_str(), args...);
		}
		if (_console) {
			_console->error(s.c_str(), args...);
		}
	}

	template<typename... Args>
	void info(const std::string& preamble, const char *fmt, const Args &... args)
	{
		std::string s = preamble + fmt;
		if (_file_all) {
			_file_all->info(s.c_str(), args...);
		}
		if (_file_all_pure) {
			_file_all_pure->info(s.c_str(), args...);
		}
		if (_console) {
			_console->info(s.c_str(), args...);
		}
	}

	template<typename... Args>
	void warn(const std::string& preamble, const char *fmt, const Args &... args)
	{
		std::string s = preamble + fmt;
		if (_file_all) {
			_file_all->warn(s.c_str(), args...);
		}
		if (_file_all_pure) {
			_file_all_pure->warn(s.c_str(), args...);
		}
		if (_console) {
			_console->warn(s.c_str(), args...);
		}
	}

	template<typename... Args>
	void trace(const std::string& preamble, const char *fmt, const Args &... args)
	{
		std::string s = preamble + fmt;
		if (_file_all) {
			_file_all->trace(s.c_str(), args...);
		}
		if (_file_all_pure) {
			_file_all_pure->trace(s.c_str(), args...);
		}
		if (_console) {
			_console->trace(s.c_str(), args...);
		}
	}

	template<typename... Args>
	void fatal(const std::string& preamble, const std::string& file, int line, const char *fmt, const Args &... args)
	{
		std::string s = preamble + fmt;
        std::string fileInfo = "    " + file + ":" + std::to_string(line);
		if (_file_all) {
			_file_all->critical(s.c_str(), args...);
			_file_all->critical(fileInfo);
		}
		if (_file_all_pure) {
			_file_all_pure->critical(s.c_str(), args...);
			_file_all_pure->critical(fileInfo);
		}
		if (_file_err) {
			_file_err->critical(s.c_str(), args...);
			_file_err->critical(fileInfo);
		}
		if (_file_err_pure) {
			_file_err_pure->critical(s.c_str(), args...);
			_file_err_pure->critical(fileInfo);
		}
		if (_console) {
			_console->critical(s.c_str(), args...);
			_console->critical(fileInfo);
		}
		flush();

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		exit(-1);
	}

	void flush() noexcept{
		if (_file_all) {
			_file_all->flush();
		}
		if (_file_err) {
			_file_err->flush();
		}
		if (_file_all_pure) {
			_file_all_pure->flush();
		}
		if (_file_err_pure) {
			_file_err_pure->flush();
		}
	}

private:
	//param_t _param;
	cfg_t _param;
	std::shared_ptr<spdlog::logger> _file_all;
	std::shared_ptr<spdlog::logger> _file_all_pure;
	std::shared_ptr<spdlog::logger> _file_err;
	std::shared_ptr<spdlog::logger> _file_err_pure;

	std::map<std::string, std::shared_ptr<spdlog::logger>> _file_loggers;

	std::shared_ptr<spdlog::logger> _console;

	static std::string _time_base;
	static std::filesystem::path _file_folder;

	std::mutex _mux;
private:
	static void init_time_base() {
		char upTime[64];
		time_t utc;
		time(&utc);
		utc += 3600 * 8;
		struct tm ltm = *gmtime(&utc);
		strftime(upTime, 64, "%Y_%m_%d__%H_%M_%S", &ltm);
		_time_base = std::string(upTime);
	}

	void init_file_folder(void);

	void init_console();

	void init_file(const std::string& name_preamble, bool enable, bool pure, bool error);

	void init_files_all();

	void init_files_err();

};

#define	TRACE(...) {S4::s4logger::pInstance()->trace("[global]", __VA_ARGS__);}
#define	INFO(...) {S4::s4logger::pInstance()->info("[global]", __VA_ARGS__);}
#define	WARN(...) {S4::s4logger::pInstance()->warn("[global]", __VA_ARGS__);S4::s4logger::pInstance()->flush();}
#define	ERR(...) {S4::s4logger::pInstance()->err("[global]", __VA_ARGS__);S4::s4logger::pInstance()->flush();}
#define	FATAL(...) {S4::s4logger::pInstance()->fatal("[global]", __FILE__, __LINE__, __VA_ARGS__);}

#define CREATE_LOCAL_LOGGER(NAME) static const char * __LCL_NAME__ = "["#NAME"] ";

#define LCL_TRAC(...) {S4::s4logger::pInstance()->trace(__LCL_NAME__, __VA_ARGS__);}
#define	LCL_INFO(...) {S4::s4logger::pInstance()->info(__LCL_NAME__, __VA_ARGS__);}
#define	LCL_WARN(...) {S4::s4logger::pInstance()->warn(__LCL_NAME__, __VA_ARGS__);S4::s4logger::pInstance()->flush();}
#define	LCL_ERR(...) {S4::s4logger::pInstance()->err(__LCL_NAME__, __VA_ARGS__);S4::s4logger::pInstance()->flush();}
#define	LCL_FATAL(...) {S4::s4logger::pInstance()->fatal(__LCL_NAME__, __FILE__, __LINE__, __VA_ARGS__);}

}//namespace S4