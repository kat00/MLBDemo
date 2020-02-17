//
//  CMLBJson.hpp
//  MLB-mobile
//
//  Created by Kristina Theroux on 2/13/20.
//

#ifndef CMLBJson_hpp
#define CMLBJson_hpp

#include <stdio.h>
#include "constants.h"
#include "include/rapidjson/document.h"
#include "CJsonQuery.h"

class CMLBJson : public CJsonQuery
{
public:
    CMLBJson();
    virtual ~CMLBJson();

    void SendRequest(int month, int day, int year);
    void GetGames(std::vector<MLB_GAME*>& games);
    
    int GetAwayTeamID(int game);
    int GetHomeTeamID(int game);
    int GetGameCount();
    
    int GetHomeScore(int game);
    int GetAwayScore(int game);
    
    int GetHomeWins(int game);
    int GetHomeLosses(int game);
    int GetAwayWins(int game);
    int GetAwayLosses(int game);
    
    const char* GetWinner(int game);
    const char* GetLoser(int game);
    const char* GetSave(int game);
    
    const char* GetUpperLabel(int game);
    const char* GetLowerLabel(int game);

private:

};


#endif //#define CMLBJson_hpp

