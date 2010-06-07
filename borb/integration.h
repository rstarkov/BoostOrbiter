//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

namespace borb { namespace integration {

    template <class T>
    class VesselAttached
    {
    public:
        typedef typename std::map<VESSEL*, std::shared_ptr<T>>::iterator iterator;
        inline iterator begin() { return _map.begin(); }
        inline iterator end() { return _map.end(); }

        inline void Clear()
        {
            _map.clear();
        }

        inline std::shared_ptr<T> Get(VESSEL* vessel)
        {
            iterator found = _map.find(vessel);
            if (found == _map.end())
            {
                shared_ptr<T> val(new T(vessel));
                _map[vessel] = val;
                return val;
            }
            else
            {
                return found->second;
            }
        }

        inline void PreStep(double simt, double simdt, double mjd)
        {
            for (VesselAttached<AutoTrimComputer>::iterator iter = Glob.Computers.begin(); iter != Glob.Computers.end(); iter++)
                iter->second->PreStep(simt, simdt, mjd);
        }

    private:
        std::map<VESSEL*, std::shared_ptr<T>> _map;
    };

} }
