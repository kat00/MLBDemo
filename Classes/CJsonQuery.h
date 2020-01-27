//
// Created by Kristina Theroux on 1/23/20.
//

#ifndef DISNEYPROTO_CJSONQUERY_H
#define DISNEYPROTO_CJSONQUERY_H

#include "include/rapidjson/document.h"

class CJsonQuery
{
public:
    CJsonQuery();
    ~CJsonQuery();

    void SendRequest(int month, int day, int year);
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

    rapidjson::Document mDoc;

};


#endif //DISNEYPROTO_CJSONQUERY_H
