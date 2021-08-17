"""Adds repostories/archives."""

########################################################################
# DO NOT EDIT THIS FILE unless you are inside the
# https://github.com/3rdparty/bazel-rules-leveldb repository. If you
# encounter it anywhere else it is because it has been copied there in
# order to simplify adding transitive dependencies. If you want a
# different version of bazel-rules-leveldb follow the Bazel build
# instructions at https://github.com/3rdparty/bazel-rules-leveldb.
########################################################################

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# libuv
########################################################################
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

def repos(external = True, repo_mapping = {}):
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.2.0.tar.gz",
            sha256 = "d54742ffbdc6924f222d2179f0e10e911c5c659c4ae74158e9fe827aad862ac6",
            strip_prefix = "rules_foreign_cc-0.2.0",
            repo_mapping = repo_mapping,
        )

    if external and "com_github_3rdparty_bazel_rules_leveldb" not in native.existing_rules():
        git_repository(
            name = "com_github_3rdparty_bazel_rules_leveldb",
            remote = "https://github.com/3rdparty/bazel-rules-leveldb",
            commit = "a735c49c87f239b18ccbe7cfcfcd620566106202",
            shallow_since = "1620093785 -0700",
            repo_mapping = repo_mapping,
        )

    if external and "libuv" not in native.existing_rules():
        http_archive(
            name = "libuv",
            build_file_content = all_content,
            sha256 = "371e5419708f6aaeb8656671f89400b92a9bba6443369af1bb70bcd6e4b3c764",
            strip_prefix = "libuv-1.42.0",
            url = "https://github.com/libuv/libuv/archive/refs/tags/v1.42.0.tar.gz",
            repo_mapping = repo_mapping,
        )

    if external and "libcurl" not in native.existing_rules():
        http_archive(
            name = "libcurl",
            build_file_content = all_content,
            sha256 = "ffa8f79f68dd77b08987ce16acd1f292875df8ab3bf7e3654f98d62b445ebd9a",
            strip_prefix = "curl-curl-7_78_0",
            url = "https://github.com/curl/curl/archive/refs/tags/curl-7_78_0.tar.gz",
            repo_mapping = repo_mapping,
        )

    if external and "com_github_gflags_gflags" not in native.existing_rules():
        http_archive(
            name = "com_github_gflags_gflags",
            url = "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
            sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
            strip_prefix = "gflags-2.2.2",
            repo_mapping = repo_mapping,
        )

    if external and "com_github_google_glog" not in native.existing_rules():
        http_archive(
            name = "com_github_google_glog",
            sha256 = "eede71f28371bf39aa69b45de23b329d37214016e2055269b3b5e7cfd40b59f5",
            strip_prefix = "glog-0.5.0",
            url = "https://github.com/google/glog/archive/refs/tags/v0.5.0.tar.gz",
            repo_mapping = repo_mapping,
        )

    if external and "gtest" not in native.existing_rules():
        http_archive(
            name = "gtest",
            url = "https://github.com/google/googletest/archive/release-1.10.0.tar.gz",
            sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
            strip_prefix = "googletest-release-1.10.0",
            repo_mapping = repo_mapping,
        )