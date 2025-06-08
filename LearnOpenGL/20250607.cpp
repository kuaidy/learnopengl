#include <coroutine>


template<typename T>
struct  Generator
{
	struct promise_type;
	//操作协程的指针
	using handle_type = std::coroutine_handle<promise_type>;

	struct promis_type {
		T current_value;
		std::suspend_always yield_value(T value) {
			current_value = value;
			return {};
		}
		Generator get_return_object() {
			return Generator{ handle_type::from_promise(*this) };
		}
		std::suspend_always initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() { }
	};

	handle_type coro;

	Generator(handle_type h) :coro(h) {};
	~Generator() { if (coro) coro.destroy(); }

	bool next() {
		coro.resume();
		return !coro.done();
	}
	T value() const {
		return coro.promise().current_value;
	}
};

int main20250607() {

	return 0;
}