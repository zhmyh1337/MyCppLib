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
			fprintf(m_stream, "Timer \"%s\": %lld %s passed.\n",
				m_name,
				std::chrono::duration_cast<TimeUnit>(ChronoClock::now() - m_startPoint).count(),
				m_timeUnitAbbreviation
			);
		}

	private:
		const char* m_name;
		const char* m_timeUnitAbbreviation;
		FILE* m_stream;
		const bool m_triggerOnDestructed;
		const ChronoClock::time_point m_startPoint;
	};
}
