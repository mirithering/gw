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
        "//entities",
        "//skills",
        "//weapon",
    ],
)

cc_test(
    name = "engine_test",
    size = "small",
    srcs = ["engine_test.cpp"],
    deps = [
        ":engine",
        "//test",
        "@com_google_googletest//:gtest_main",
    ],
)

test_suite(
    name = "tests",
    tests = [
        "engine_test",
        "//armor:test",
        "//base:test",
        "//conditions:test",
        "//entities:test",
        "//skills:test",
        "//weapon:test",
    ],
)
