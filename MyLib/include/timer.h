#pragma once

#include <chrono>

namespace my
{
	template<class TimeUnit = std::chrono::milliseconds>
	class timer
	{
	private:
		using ChronoClock = std::chrono::high_resolution_clock;

	public:
		timer(const char* name = "unnamed", bool triggerOnDestructed = false, const char* timeUnitAbbreaviation = "ms", FILE* stream = stderr)
			: m_name(name), m_timeUnitAbbreviation(timeUnitAbbreaviation), m_stream(stream), m_triggerOnDestructed(triggerOnDestructed),
			m_startPoint(ChronoClock::now()) {}

		~timer()
		{
			if (m_triggerOnDestructed)
			{
				fprintf(m_stream, "~");
				time_passed();
			}
		}

		void time_passed()
		{
			const char* valueFormatSpecifier = std::is_same_v<TimeUnit::rep, int> ? "d" : "lld";
			const char preFormat[] = "Timer \"%%s\": %%%s %%s passed.\n";
			char format[sizeof(preFormat) - 2];
			sprintf_s(format, preFormat, valueFormatSpecifier);

			fprintf(m_stream, format, m_name, time_passed_value(), m_timeUnitAbbreviation);
		}

		decltype(auto) time_passed_value()
		{
			return std::chrono::duration_cast<TimeUnit>(ChronoClock::now() - m_startPoint).count();
		}

	private:
		const char* m_name;
		const char* m_timeUnitAbbreviation;
		FILE* m_stream;
		const bool m_triggerOnDestructed;
		const ChronoClock::time_point m_startPoint;
	};
}
