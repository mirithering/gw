cc_library(
    name = "base",
    srcs = [
        "attribute.cpp",
        "clock.cpp",
        "effect.cpp",
        "profession.cpp",
        "random.cpp",
        "units.cpp",
    ],
    hdrs = [
        "attribute.h",
        "clock.h",
        "effect.h",
        "event.h",
        "function_list.h",
        "logging.h",
        "profession.h",
        "random.h",
        "units.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "clock_test.cpp",
        "effect_test.cpp",
        "event_test.cpp",
        "function_list_test.cpp",
        "random_test.cpp",
        "units_test.cpp",
    ],
    deps = [
        "base",
        "@com_google_googletest//:gtest_main",
    ],
)
