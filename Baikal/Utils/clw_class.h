#pragma once

#include <string>
#include "CLW.h"

namespace Baikal
{
    class ClwClass
    {
    public:
        ClwClass(CLWContext context,
                 std::string const& cl_file,
                 std::string const& opts = "");
        virtual ~ClwClass() = default;

        void Rebuild(std::string const& opts);
    protected:
        CLWContext GetContext() const { return m_context; }
        CLWKernel GetKernel(std::string const& name);
        std::string GetBuildOpts() const { return m_buildopts; }
        
        
    private:
        CLWContext m_context;
        CLWProgram m_program;
        std::string m_buildopts;
        std::string m_cl_file;
        std::string m_opts;
    };
    
    inline ClwClass::ClwClass(
        CLWContext context,
        std::string const& cl_file,
        std::string const& opts
                              )
    : m_context(context)
    , m_cl_file(cl_file)
    , m_opts(opts)
    {
        m_buildopts.append(" -cl-mad-enable -cl-fast-relaxed-math "
                         "-cl-std=CL1.2 -I . ");

        m_buildopts.append(
#if defined(__APPLE__)
                         "-D APPLE "
#elif defined(_WIN32) || defined (WIN32)
                         "-D WIN32 "
#elif defined(__linux__)
                         "-D __linux__ "
#else
                         ""
#endif
                         );

        auto cmdopts = m_buildopts;
        cmdopts.append(opts);
        m_program = CLWProgram::CreateFromFile(cl_file.c_str(),
                                               cmdopts.c_str(), m_context);
    }

    inline void ClwClass::Rebuild(std::string const& opts)
    {
        if (m_opts != opts)
        {
            auto cmdopts = m_buildopts;
            cmdopts.append(opts);
            m_opts = opts;
            m_program = CLWProgram::CreateFromFile(m_cl_file.c_str(),
                cmdopts.c_str(), m_context);
        }
    }

    inline CLWKernel ClwClass::GetKernel(std::string const& name)
    {
        return m_program.GetKernel(name);
    }
}
