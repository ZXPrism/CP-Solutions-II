# Template tests

From the repository root:

```sh
xmake check -P templates
```

Or run `xmake check` inside `templates`.

This discovers `.cpp` files containing `#ifdef CP_TEMPLATE_TEST`, builds their
embedded doctest tests with C++17 and exceptions enabled, and runs them together.
Doctest reports the test and assertion counts. A build or test failure returns a
nonzero exit code. Build files are placed in `templates/.test-tmp/` and
removed after success or failure; forced process termination can interrupt cleanup.
The first run downloads doctest through xmake. Its shared dependency cache is kept.

Add tests directly after each template:

```cpp
#ifdef CP_TEMPLATE_TEST
#include <doctest/doctest.h>

TEST_CASE("MyTemplate: example") {
    // CHECK(...);
    // CHECK_THROWS(...); // Any exception is enough; type and text are unrestricted.
}
#endif
```

The test runner supplies `main()`. Normal use of the template does not need doctest
or define `CP_TEMPLATE_TEST`. Invalid inputs must throw; tests do not require an
exception type, message, or preservation of state after an exception.
