#include "common/s4logger.h"

namespace S4 {


std::string s4logger::_time_base = "null";
std::filesystem::path s4logger::_file_folder = ".";

static time_t last_err_time_ = 0;
inline void err_handler(const std::string &msg)
{
    auto now = time(nullptr);
    if (now - last_err_time_ < 60)
    {
        return;
    }
    last_err_time_ = now;
    fmt::print(stderr, "[*** LOGGER ERROR ***]  {}\n", msg);
	throw std::exception(std::logic_error("logger error!"));	//logger fail is critical error!

}

s4logger::s4logger()
{
	// _param = new glb_conf_ctx_t::logger_t;
}

void s4logger::init()
{
	flush();

	init_console();

	init_file_folder();
	init_files_all();
	init_files_err();
}

void s4logger::init(const s4logger::cfg_t& p) noexcept {
	_param = p;
    init();
}


void s4logger::init_file_folder(void) {
	_file_folder = _param.save_path;
	if (std::filesystem::exists(_file_folder)) {
		if (!std::filesystem::is_directory(_file_folder)) {
			if (!std::filesystem::create_directory(_file_folder)) {
				std::cerr << "create_directory(" << _file_folder << ") fail!" << std::endl;
				throw std::runtime_error("logger init fail!");
			}
		}
	}
	else
	{
		if (!std::filesystem::create_directories(_file_folder)) {
			std::cerr << "create_directory(" << _file_folder << ") fail!" << std::endl;
			throw std::runtime_error("logger init fail!");
		}
	}
}

void s4logger::init_console() {
	if (_param.enable_console) {
		if (!_console) {
			_console = spdlog::create<spdlog::sinks::stdout_color_sink_mt>("console");
			_console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %^%v%$");
		}
		_console->set_level(_param.level);
	    _console->set_error_handler(err_handler);
	}
	else if (_console) {
		spdlog::drop("console");
		_console = nullptr;
	}
}

void s4logger::init_file(const std::string& name_preamble, bool enable, bool pure, bool error) {
	std::shared_ptr<spdlog::logger> pLogger;
	std::string name = name_preamble + _time_base;
	if (error) {
		name += "_err";
	}
	if (pure) {
		name += "_pure";
	}

	std::filesystem::path file_path = _file_folder / (name + ".log");

	if (!enable) {
		if (_file_loggers.count(name) != 0) {
			spdlog::drop(name);
			_file_loggers.erase(name);
		}
		return;
	}

	//enable
	if (_file_loggers.count(name) != 0) {
		pLogger = _file_loggers.at(name);
		if (((spdlog::sinks::rotating_file_sink_mt*)spdlog::get(name)->sinks()[0].get())->filename() != file_path.string()) {	//redirect file path
			_file_loggers.erase(name);
			pLogger = nullptr;
		}
	}

	if (!pLogger) {
		pLogger = spdlog::create<spdlog::sinks::rotating_file_sink_mt>(name, file_path.string(), _param.max_file_size_MB * LOG_SIZE_MB, _param.max_files);
		_file_loggers[name] = pLogger;
		//spdlog::sinks::rotating_file_sink_mt* raw = (spdlog::sinks::rotating_file_sink_mt*)pLogger->sinks()[0].get();
		//std::cout << raw->filename() << std::endl;
	}
	if (pure) {
		pLogger->set_pattern("[%l] %^%v%$");
	}
	else {
		pLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %^%v%$");
	}

	if (error) {
		pLogger->set_level(spdlog::level::err);
	}
	else {
		pLogger->set_level(_param.level);
	}
	pLogger->set_error_handler(err_handler);
}

void s4logger::init_files_all() {
	init_file(_param.file_preamble, _param.enable_file_all, false, false);
	init_file(_param.file_preamble, _param.enable_file_all_pure, true, false);

	if (_file_loggers.count(_param.file_preamble + _time_base)) {
		_file_all = _file_loggers.at(_param.file_preamble + _time_base);
	}
	else {
		_file_all = nullptr;
	}
	if (_file_loggers.count(_param.file_preamble + _time_base + "_pure")) {
		_file_all_pure = _file_loggers.at(_param.file_preamble + _time_base + "_pure");
	}
	else {
		_file_all_pure = nullptr;
	}
}

void s4logger::init_files_err() {
	init_file(_param.file_preamble, _param.enable_file_err, false, true);
	init_file(_param.file_preamble, _param.enable_file_err_pure, true, true);
	if (_file_loggers.count(_param.file_preamble + _time_base + "_err")) {
		_file_err = _file_loggers.at(_param.file_preamble + _time_base + "_err");
	}
	else {
		_file_err = nullptr;
	}
	if (_file_loggers.count(_param.file_preamble + _time_base + "_err_pure")) {
		_file_err_pure = _file_loggers.at(_param.file_preamble + _time_base + "_err_pure");
	}
	else {
		_file_err_pure = nullptr;
	}
}


}//namespace S4
