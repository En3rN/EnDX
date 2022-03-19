#include "logger.h"
namespace En3rN::DX
{
	Logger::handle Logger::logger(new Logger(Logger::Level::Debug));
};
