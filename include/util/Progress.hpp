#ifndef _PROGRESS_H
#define _PROGRESS_H

#include "text_color.hpp"

class ProgressPrint
{
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO info;
	textColor console;

	int counter_max;
	int step;
	int current_i;
	std::chrono::system_clock::time_point start_time;
public:
	inline void init()
	{
		current_i = 0;
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	ProgressPrint( int n )
	{
		init();
		counter_max = n;
	}
	ProgressPrint()
	{
		init();
		newbar();
	}

	inline void newbar()
	{
		console.color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("0%%   10   20   30   40   50   60   70   80   90   100%%\n");
		GetConsoleScreenBufferInfo(hStdout, (PCONSOLE_SCREEN_BUFFER_INFO)&info);
		printf("|----|----|----|----|----|----|----|----|----|----|\n\r"); fflush(stdout);
		console.reset();
		if (current_i > 0 ) print(current_i);
	}
	inline void start()
	{
		start_time = std::chrono::system_clock::now();
		newbar();
		step = counter_max/50;
		if (step == 0) step = 1;
	}
	inline void end()
	{
		printf("\r");
		SetConsoleCursorPosition(hStdout, info.dwCursorPosition);
		console.color(FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		console.printf("###################################################");
		console.color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		console.clear_line(5);
		console.printf("\n => 100%%!!          \n");
		console.reset();
		fflush(stdout);
	}

	inline void print(int i )
	{
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> sec = std::chrono::system_clock::now() - start_time;
		double duration = sec.count(); // íPà ÇÕ[ïb]

		if ( i % step == 0 || duration > 10.0)
		{
			current_i = i;
			double d = 100.0*(double)i / (double)(counter_max - 1);

			printf("\r");
			SetConsoleCursorPosition(hStdout, info.dwCursorPosition);
			console.color(FOREGROUND_RED | FOREGROUND_GREEN /*| FOREGROUND_INTENSITY*/ | BACKGROUND_RED | BACKGROUND_GREEN /*| BACKGROUND_INTENSITY*/);
			for (int ii = 0; ii < d/2; ii++)
			{
				console.printf("#");
			}
			if (i / step == 0) console.printf("#");
			console.reset();

			console.color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			if ( d < 0.1)
			{
				console.printf("\n=>(%d/%d) %.1f%%", i, counter_max - 1, d);
			}
			else
			{
				console.printf("\n=>(%d/%d) %.3f%%", i, counter_max - 1, d);
			}
			console.reset();
			//if ( i % 2 ) console.printf("*");else console.printf(" ");
			start_time = end;
			fflush(stdout);
			console.reset();
		}
	}
};

class measurement_time
{
	std::chrono::system_clock::time_point start_;
	std::chrono::system_clock::time_point end;
public:
	measurement_time()
	{
		start_ = std::chrono::system_clock::now();
	}

	inline void start()
	{
		start_ = std::chrono::system_clock::now();
	}
	inline void stop()
	{
		end = std::chrono::system_clock::now();  // åvë™èIóπéûä‘
	
		double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start_).count();
		printf("%f[milliseconds]\n", elapsed);
	}
	
};

#endif