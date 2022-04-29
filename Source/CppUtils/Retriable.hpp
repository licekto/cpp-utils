#pragma once

#include <atomic>
#include <functional>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>

namespace CppUtils
{
	struct RetriableLimits
	{
		static inline constexpr uint32_t MinRetries = 1;
		static inline constexpr uint32_t MaxRetries = 5;
        static inline constexpr auto  MinPeriodMs = std::chrono::milliseconds(10);
        static inline constexpr auto MaxPeriodMs = std::chrono::milliseconds(60 * 1000);

		static void CheckRetries(const uint32_t retries)
		{
			if (retries < MinRetries)
			{
                throw std::invalid_argument("Retriable with 0 attempts does not make sense. Function will not be executed at all.");
			}
			if (retries > MaxRetries)
			{
                std::string msg = "Each attempt is running in an OS thread which is detached if the limit is exceeded. ";
                msg += "There must be no more than " + std::to_string(MaxRetries) + " running threads.";
                throw std::invalid_argument(msg);
			}
		}

        static void CheckPeriod(const std::chrono::milliseconds period)
		{
            if (period < MinPeriodMs)
			{
                throw std::invalid_argument("Retry period of " + std::to_string(period.count()) + "ms might be too short.");
			}
            if (period > MaxPeriodMs)
			{
                throw std::invalid_argument("Retry period of " + std::to_string(period.count()) + "ms might be too long.");
			}
		}
	};

	template <typename ReturnTypeT, typename ...ArgsT>
	class Retriable
	{
	public:
        using Function = std::function<ReturnTypeT(const ArgsT&... args)>;

        Retriable(const std::chrono::milliseconds period_, const uint32_t retries_, Function&& function_)
            : period(period_)
			, retries(retries_)
			, function(std::move(function_))
		{
            RetriableLimits::CheckPeriod(period);
			RetriableLimits::CheckRetries(retries);
		}

		Retriable(const Retriable&) = delete;
		Retriable& operator=(const Retriable&) = delete;
		Retriable(Retriable&&) = delete;
		Retriable& operator=(Retriable&&) = delete;

        ReturnTypeT Run(const ArgsT&... args)
		{
			struct Control
			{
				std::atomic_bool detached{ false }, finished{ false };
				std::exception_ptr exception{};
			};

			ReturnTypeT finalResult{};

			bool succeeded{ false };
			uint32_t attempt;
			for (attempt = 0; attempt < retries; ++attempt)
			{
				auto control = std::make_shared<Control>();

                std::thread t([&finalResult, control, this, &args...]()
				{
					try
					{
                        auto result = function(args...);
						if (!control->detached)
						{
							finalResult = std::move(result);
						}
					}
					catch (...)
					{
						if (!control->detached)
						{
							control->exception = std::current_exception();
						}
					}
					control->finished = true;
				});

                std::this_thread::sleep_for(period);

				if (control->finished)
				{
					if (control->exception)
					{
						t.join();
						std::rethrow_exception(control->exception);
					}
					succeeded = true;
					t.join();
					break;
				}
				control->detached = true;
				t.detach();
                // Warning: "Attempt " << attempt << " has not succeeded, detaching the thread";
			}

			if (!succeeded)
			{
                throw std::invalid_argument("No retry attempt succeeded, number of attempts: " + std::to_string(retries));
			}

            // Info: "Function succeeded after " << attempt << " attempts, returning the result";
			return finalResult;
		}

	private:
        const std::chrono::milliseconds period;
		const uint32_t retries;
		Function function;
	};
}
