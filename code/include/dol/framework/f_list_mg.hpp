#pragma once
#include <types.h>
#include <dol/framework/f_profile.hpp>
#include <dol/cLib/c_list.hpp>
#include <dol/framework/f_base_id.hpp>
#include <dol/framework/f_list_nd.hpp>

class fBase_c;

/// @brief A base list, made of fLiNdBa_c nodes.
/// @ingroup framework dol
/// @note The class name stands for "List Manager (with) Base".
class fLiMgBa_c : public cListMg_c {
public:

    /**
     * @brief Counts the number of bases using the given profile name.
     *
     * @param profName The profile name.
     * @return How many bases were found.
     */
    int countNodeByProfName(ProfileName profName) const;

    /**
     * @brief Searches for a base with the given ID.
     *
     * @param id The ID to search for.
     * @return The node belonging to such base, or @p nullptr.
     */
    const fLiNdBa_c *searchNodeByID(fBaseID_e id) const;

    inline fLiNdBa_c *getFirst() const {
        return (fLiNdBa_c *) cListMg_c::getFirst();
    }

    inline fLiNdBa_c *getLast() const {
        return (fLiNdBa_c *) cListMg_c::getLast();
    }
};
