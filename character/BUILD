cc_library(
    name = "character",
    srcs = [
        "action.cpp",
        "character.cpp",
        "damage.cpp",
        "skill.cpp",
    ],
    hdrs = [
        "action.h",
        "character.h",
        "damage.h",
        "skill.h",
        "stance.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//armor",
        "//base",
        "//weapon",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "action_test.cpp",
        "character_test.cpp",
    ],
    deps = [
        "character",
        "//skills",
        "@com_google_googletest//:gtest_main",
    ],
)
