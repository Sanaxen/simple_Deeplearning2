#ifndef _PROGRESS_H
#define _PROGRESS_H

class ProgressPrint
{
	int counter_max;
	int step;
	std::chrono::system_clock::time_point start_time;
public:
	ProgressPrint( int n )
	{
		counter_max = n;
	}
	ProgressPrint()
	{
		newbar();
	}

	inline void newbar() const
	{
		printf("0%%   10   20   30   40   50   60   70   80   90   100%%\n");
		printf("|----|----|----|----|----|----|----|----|----|----|\n\r"); fflush(stdout);
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
		printf("################################################### => 100%%!!          \n");
		fflush(stdout);
	}

	inline void print(int i )
	{
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> sec = std::chrono::system_clock::now() - start_time;
		double duration = sec.count(); // íPà ÇÕ[ïb]

		if ( i % step == 0 || duration > 10.0)
		{
			double d = 100.0*(double)i / (double)(counter_max - 1);

			printf("\r");
			for (int ii = 0; ii < d/2; ii++)
			{
				printf("#");
			}
			if (i / step == 0) printf("#");

			if ( d < 0.1)
			{
				printf("=>(%d/%d) %.1f%%", i, counter_max - 1, d);
			}
			else
			{
				printf("=>(%d/%d) %.3f%%", i, counter_max - 1, d);
			}
			if ( i % 2 ) printf("*");else printf(" ");
			start_time = end;
			fflush(stdout);
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