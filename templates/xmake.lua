set_project("cp-templates")
set_xmakever("2.9.0")

task("check")
    set_category("plugin")
    set_menu {
        usage = "xmake check -P templates",
        description = "Build and run embedded doctest tests, then remove temporary build files.",
        options = {}
    }
    on_run(function ()
        local root = path.absolute(os.projectdir())
        local sources = {}
        for _, file in ipairs(os.files(path.join(root, "**.cpp"))) do
            if io.readfile(file):find("#ifdef CP_TEMPLATE_TEST", 1, true) then
                table.insert(sources, file)
            end
        end
        table.sort(sources)
        assert(#sources > 0, "No embedded template tests found.")

        -- All per-run project and build files stay in templates/.test-tmp.
        -- The shared xmake dependency cache is retained.
        local scratch = path.join(root, ".test-tmp")
        assert(path.directory(scratch) == root, "Unsafe temporary path.")
        assert(not os.exists(scratch), "Temporary path already exists.")
        os.mkdir(scratch)
        try {
            function ()
                local config = {
                    'set_project("cp-template-tests")',
                    'set_languages("c++17")',
                    'add_rules("mode.debug")',
                    'add_requires("doctest 2.4.12")',
                    'target("template-tests")',
                    '    set_kind("binary")',
                    '    set_exceptions("cxx")',
                    '    add_defines("CP_TEMPLATE_TEST")',
                    '    add_packages("doctest")',
                    '    add_files("test_main.cpp")'
                }
                for _, file in ipairs(sources) do
                    table.insert(config, '    add_files(' .. string.format("%q", file) .. ')')
                end
                io.writefile(path.join(scratch, "xmake.lua"), table.concat(config, "\n"))
                io.writefile(path.join(scratch, "test_main.cpp"),
                    '#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n#include <doctest/doctest.h>\n')
                print("Testing %d template file(s).", #sources)
                local options = {curdir = scratch}
                os.execv(os.programfile(), {"f", "-P", scratch, "-m", "debug", "--ccache=n", "-o", path.join(scratch, "build"), "-y"}, options)
                os.execv(os.programfile(), {"build", "-P", scratch, "-y"}, options)
                os.execv(os.programfile(), {"run", "-P", scratch, "template-tests"}, options)
            end,
            finally {
                function (ok, errors)
                    -- Only remove the directory created by this invocation.
                    os.cd(root)
                    assert(path.directory(scratch) == root, "Unsafe cleanup path.")
                    os.rm(scratch)
                    print("Temporary build files removed.")
                    if not ok then
                        raise(errors)
                    end
                end
            }
        }
    end)
