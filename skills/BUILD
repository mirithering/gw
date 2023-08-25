cc_library(
    name = "skills",
    srcs = [
        "bonettis_defense.cpp",
        "desperate_strike.cpp",
    ],
    hdrs = [
        "bonettis_defense.h",
        "desperate_strike.h",
        "jaizenju_strike.h",
        "pure_strike.h",
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
        "bonettis_defense_test.cpp",
    ],
    deps = [
        "skills",
        "@com_google_googletest//:gtest_main",
    ],
)
