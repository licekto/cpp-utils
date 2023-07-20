cmake_minimum_required(VERSION 3.21)

set(PRECOMPILED_HEADERS
        # System headers
        <assert.h> <errno.h> <net/if_arp.h>  <netinet/in.h> <pthread.h> <signal.h> <string.h> <sys/ioctl.h> <sys/socket.h> <sys/types.h>
        # STL headers
        <algorithm> <any> <array> <atomic> <cassert> <cerrno> <chrono> <condition_variable> <cstdarg> <cstdint> <cstdlib>
        <cstring> <ctime> <deque> <exception> <fstream> <functional> <future> <iomanip> <iostream> <istream> <limits>
        <locale> <map> <memory> <mutex> <optional> <ostream> <queue> <random> <regex> <set> <sstream> <stdexcept>
        <string> <string_view> <system_error> <thread> <tuple> <typeindex> <typeinfo> <type_traits> <unordered_map>
        <unordered_set> <utility> <variant> <vector>)
