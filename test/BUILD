cc_library(
    name = "test",
    srcs = [
        "test.cpp",
    ],
    hdrs = [
        "test.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//entities",
        "//weapon",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "tests",
    size = "small",
    srcs = [
    ],
    deps = [
        "@com_google_googletest//:gtest_main",
    ],
)
