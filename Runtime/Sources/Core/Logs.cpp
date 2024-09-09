#include <string>
#include <string_view>

#include <Core/Logs.h>
#include <Utils/Ansi.h>
#include <Core/EventBase.h>
#include <Core/EventBus.h>
#include <Core/MessageBox.h>

namespace kbh
{
	namespace Internal
	{
		struct FatalErrorEvent : public EventBase
		{
			Event What() const override { return Event::FatalErrorEventCode; }
		};
	}

	void Logs::Report(LogType type, std::string message)
	{
		Report(type, 0, {}, {}, std::move(message));
	}

	void Logs::Report(LogType type, unsigned int line, std::string_view file, std::string_view function, std::string message)
	{
		using namespace std::literals;

		#ifndef KANEL_3D_DEBUG
			if(type == LogType::Debug)
				return;
		#endif

		std::string code_infos;
		if((type == LogType::Error || type == LogType::FatalError) && !file.empty() && !function.empty())
		{
			code_infos += "{in file '"s;
			code_infos += file;
			code_infos += "', line "s + std::to_string(line) + ", in function '"s;
			code_infos += function;
			code_infos += "'} "s;
		}

		switch(type)
		{
			case LogType::Debug: std::cout << Ansi::blue << "[kanel-3D Debug] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::Message: std::cout << Ansi::blue << "[kanel-3D Message] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::Warning: std::cout << Ansi::magenta << "[kanel-3D Warning] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::Error: std::cerr << Ansi::red << "[kanel-3D Error] " << Ansi::def << code_infos << message << '\n'; break;
			case LogType::FatalError: std::cerr << Ansi::red << "[kanel-3D Fatal Error] " << Ansi::def << code_infos << message << '\n'; break;

			default: break;
		}

		#ifndef KANEL_3D_DEBUG
			if(type != LogType::Debug)
				MessageBox(message, type);
		#endif

		if(type == LogType::FatalError)
		{
			std::cout << Ansi::bg_red << "Fatal Error: emergency exit" << Ansi::bg_def << std::endl;
			EventBus::Send("__kanel-3D", Internal::FatalErrorEvent{});
		}
	}
}
