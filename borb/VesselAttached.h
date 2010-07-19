//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb {

    template <class T>
    class VesselAttached : boost::noncopyable
    {
    public:
        typedef typename std::map<VESSEL*, boost::shared_ptr<T>>::iterator iterator;

        inline iterator begin() { return _map.begin(); }
        inline iterator end() { return _map.end(); }
        inline void clear() { _map.clear(); }

        boost::shared_ptr<T> Get(VESSEL* vessel)
        {
            iterator found = _map.find(vessel);
            if (found == _map.end())
            {
                boost::shared_ptr<T> val(new T(vessel));
                _map[vessel] = val;
                return val;
            }
            else
            {
                return found->second;
            }
        }

        void PreStep(double simt, double simdt, double mjd)
        {
            for (iterator it = begin(); it != end(); it++)
                it->second->PreStep(simt, simdt, mjd);
        }

        void PostStep(double simt, double simdt, double mjd)
        {
            for (iterator it = begin(); it != end(); it++)
                it->second->PostStep(simt, simdt, mjd);
        }

        void SaveTo(ScenarioNode* node)
        {
            for (iterator it = begin(); it != end(); it++)
            {
                std::string name(it->first->GetName());
                it->second->SaveTo(&node->NamedChildren[name]);
            }
        }

        void LoadFrom(ScenarioNode* node)
        {
            for (ScenarioNode::iterator_named it = node->NamedChildren.begin(); it != node->NamedChildren.end(); it++)
            {
                OBJHANDLE vesselObj = GetVesselByName(it->first);
                if (vesselObj == NULL)
                    continue;
                VESSEL* vessel = oapiGetVesselInterface(vesselObj);
                Get(vessel)->LoadFrom(&*it->second);
            }
        }

    private:
        std::map<VESSEL*, boost::shared_ptr<T>> _map;
    };

}
