cc_library(
    name = "conditions",
    srcs = [
    ],
    hdrs = [
        "bleeding.h",
        "crippled.h",
        "deep_wound.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//character",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "deep_wound_test.cpp",
    ],
    deps = [
        "conditions",
        "//character",
        "//weapon",
        "@com_google_googletest//:gtest_main",
    ],
)
