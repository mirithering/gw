cc_library(
    name = "base",
    srcs = [
        "attribute.cpp",
        "clock.cpp",
        "profession.cpp",
        "random.cpp",
    ],
    hdrs = [
        "attribute.h",
        "clock.h",
        "profession.h",
        "random.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "clock_test.cpp",
        "random_test.cpp",
    ],
    deps = [
        "base",
        "@com_google_googletest//:gtest_main",
    ],
)
