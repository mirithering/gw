cc_library(
    name = "engine",
    srcs = [
        "engine.cpp",
    ],
    hdrs = [
        "engine.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//armor",
        "//base",
        "//character",
        "//skills",
        "//weapon",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = ["engine_test.cpp"],
    deps = [
        ":engine",
        "@com_google_googletest//:gtest_main",
    ],
)

test_suite(
    name = "tests",
    tests = [
        "test",
        "//armor:test",
        "//base:test",
        "//character:test",
        "//conditions:test",
        "//skills:test",
        "//weapon:test",
    ],
)
