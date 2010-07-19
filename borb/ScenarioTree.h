//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb {

    class ScenarioValue : boost::noncopyable, public std::stringstream
    {
    public:
        ScenarioValue()
            : had_added(false)
        {
        }

        void SkipWhitespace()
        {
            while (peek() == ' ')
                get();
        }

        template<typename T>
        void AddValue(T value)
        {
            if (had_added)
                *this << " ";
            *this << value;
            had_added = true;
        }

        void AddValue(const VECTOR3& vector)
        {
            AddValue(vector.x);
            AddValue(vector.y);
            AddValue(vector.z);
        }

        void AddValue(const char* value)
        {
            if (had_added)
                *this << " \"";
            else
                *this << "\"";

            const char* last = value;
            const char* cur = value;
            while (*cur != 0)
            {
                if (*cur == '"')
                {
                    write(last, cur - last + 1);
                    last = cur;
                }
                cur++;
            }
            if (cur - last > 0)
                write(last, cur - last);

            *this << "\"";
            had_added = true;
        }

        void AddValue(char* value)
        {
            AddValue((const char*) value);
        }

        void AddValue(const std::string& value)
        {
            AddValue(value.c_str());
        }

        template<typename T>
        T GetValue()
        {
            SkipWhitespace();
            if (eof()) throw std::exception("Attempted to ScenarioValue::GetValue but there were no more values.");
            T result;
            *this >> result;
            if (fail()) throw std::exception("Failed to ScenarioValue::GetValue, probably by trying to parse incorrect data.");
            return result;
        }

        template<>
        VECTOR3 GetValue<VECTOR3>()
        {
            VECTOR3 result;
            result.x = GetValue<double>();
            result.y = GetValue<double>();
            result.z = GetValue<double>();
        }

        template<>
        std::string GetValue<std::string>()
        {
            SkipWhitespace();
            if (eof()) throw std::exception("Attempted to ScenarioValue::GetValue but there were no more values.");
            if (peek() != '"') throw std::exception("ScenarioValue::GetValue expected a string starting with a \" character.");
            get();
            std::stringbuf result;
            while (true)
            {
                if (peek() != '"') // "get" cannot retrieve zero chars without failing (intentionally...) so need to check for that specifically
                    get(result, '"');
                if (eof()) throw std::exception("Unexpected end of value while parsing a string in ScenarioValue::GetValue.");
                get(); // the " that ended the get
                if (peek() != '"') break;
                get(); // the second part of the " escape
                result.sputc('"');
            }
            return result.str();
        }

        template<typename T>
        void GetValueIfPossible(T* destination)
        {
            try { *destination = GetValue<T>(); }
            catch (std::exception& ex) { }
        }

        template<typename T>
        T GetValueOrDefault(T default_)
        {
            try { return GetValue<T>(); }
            catch (std::exception&) { return default_; }
        }

    private:
        bool had_added;
    };

    class ScenarioNode : boost::noncopyable
    {
    public:
        typedef boost::ptr_map<std::string, ScenarioNode>::iterator iterator_named;
        typedef boost::ptr_vector<ScenarioNode>::iterator iterator_numbered;

        boost::ptr_map<std::string, ScenarioValue> Values;
        boost::ptr_map<std::string, ScenarioNode> NamedChildren;
        boost::ptr_vector<ScenarioNode> NumberedChildren;

        void SaveTo(FILEHANDLE scn);
        void LoadFrom(FILEHANDLE scn);

    private:
        void save(FILEHANDLE scn, int depth);
        void load(FILEHANDLE scn);

        void write(FILEHANDLE scn, const std::string& name, const std::string& value);
    };

}
