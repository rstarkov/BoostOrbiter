//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "ScenarioTree.h"

namespace borb {

    using namespace std;

    void ScenarioNode::SaveTo(FILEHANDLE scn)
    {
        write(scn, "SCENARIO_TREE", "");
        save(scn, 1);
        write(scn, "END_NODE", "");
    }

    void ScenarioNode::LoadFrom(FILEHANDLE scn)
    {
        char* line;
        if (!oapiReadScenario_nextline(scn, line))
            return;
        if (string(line) != "SCENARIO_TREE")
            return;
        load(scn);
    }


    void ScenarioNode::save(FILEHANDLE scn, int depth)
    {
        using boost::ptr_map;
        using boost::ptr_vector;

        string indent = string(depth * 2, ' ');

        for (ptr_map<string, ScenarioValue>::iterator it = Values.begin(); it != Values.end(); it++)
            write(scn, indent + it->first, it->second->str());

        for (ptr_map<string, ScenarioNode>::iterator it = NamedChildren.begin(); it != NamedChildren.end(); it++)
        {
            write(scn, indent + "NAMED", it->first);
            it->second->save(scn, depth + 1);
            write(scn, indent + "END_NODE", "");
        }

        for (ptr_vector<ScenarioNode>::iterator it = NumberedChildren.begin(); it != NumberedChildren.end(); it++)
        {
            write(scn, indent + "NUMBERED", "");
            it->save(scn, depth + 1);
            write(scn, indent + "END_NODE", "");
        }
    }

    void ScenarioNode::write(FILEHANDLE scn, const string& name, const string& value)
    {
        // could also const_cast in theory but who knows...
        vector<char> copyName(name.begin(), name.end());
        copyName.push_back(0);
        vector<char> copyData(value.begin(), value.end());
        copyData.push_back(0);
        oapiWriteScenario_string(scn, &copyName[0], &copyData[0]);
        // (alternatively use oapiWriteLine)
    }

    void ScenarioNode::load(FILEHANDLE scn)
    {
        char* line;
        while (oapiReadScenario_nextline(scn, line)) // returns false as soon as it encounters "END" (consuming it)
        {
            string str = line;
            size_t spacepos = str.find(' ');
            string left, right;
            if (spacepos == string::npos)
            {
                left = str;
            }
            else
            {
                left = str.substr(0, spacepos);
                right = str.substr(spacepos + 1);
            }

            if (left == "END_NODE")
            {
                return;
            }
            else if (left == "NAMED")
            {
                NamedChildren[right].load(scn);
            }
            else if (left == "NUMBERED")
            {
                NumberedChildren.push_back(new ScenarioNode());
                NumberedChildren.back().load(scn);
            }
            else
            {
                Values[left].str(right);
            }
        }
        throw exception("ScenarioNode::LoadFrom: unexpected end of input (2)");
    }

}
