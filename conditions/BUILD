cc_library(
    name = "conditions",
    srcs = [
    ],
    hdrs = [
        "bleeding.h",
        "crippled.h",
        "dazed.h",
        "deep_wound.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//entities",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "crippled_test.cpp",
        "dazed_test.cpp",
        "deep_wound_test.cpp",
    ],
    deps = [
        "conditions",
        "//entities",
        "//skills",
        "//test",
        "//weapon",
        "@com_google_googletest//:gtest_main",
    ],
)
