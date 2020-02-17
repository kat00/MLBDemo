//
//  CMLBJson.cpp
//  MLB-mobile
//
//  Created by Kristina Theroux on 2/13/20.
//

#include "CMLBJson.h"
#include <curl/curl.h>
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/error/en.h"

using namespace rapidjson;

const char* QUERY_ADDR = "statsapi.mlb.com";
const char* QUERY_PATH = "/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=%d-%d-%d&sportId=1";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *query)
{
    size_t realsize = size * nmemb;
    ((CJsonQuery*)query)->AddData((const char*)contents, realsize);
    return realsize;
}

CMLBJson::CMLBJson()
: CJsonQuery()
{

}

CMLBJson::~CMLBJson()
{

}

void CMLBJson::SendRequest(int month, int day, int year)
{
    char path[256] = {0};
    snprintf(path, 255, QUERY_PATH, year, month, day);

    const int buffsize = 2048;
    char buffer[buffsize] = { 0 };
    snprintf(buffer, buffsize, "http://%s/%s", QUERY_ADDR,path);

    if( SendQuery(buffer) )
    {
        ParseResult ok = mDoc.Parse(m_document_data);
        if( mDoc.HasParseError() )
        {
            fprintf(stderr, "JSON parse error: %s (%u)", GetParseError_En(ok.Code()), ok.Offset());

        }
    }
}

void CMLBJson::GetGames(std::vector<MLB_GAME*>& gamevec)
{
    if( mDoc.HasParseError() )
        return;
    
    const Value& date = mDoc["dates"];
    if( date.Size() == 0)
        return;

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    int gamecount = games.Size();
    for(int x = 0; x< gamecount; ++x)
    {
        MLB_GAME* game = new MLB_GAME();
        const Value& gameitem = games[x];
        const Value& teams = gameitem["teams"];
        const Value& home = teams["home"];
        const Value& away = teams["away"];
        
        const Value& hometeam = home["team"];
        const Value& awayteam = away["team"];
        
        game->hometeam = hometeam["id"].GetInt();
        game->awayteam = awayteam["id"].GetInt();
        
        gamevec.push_back(game);
        
        //status
        const Value& status = gameitem["status"];
        if( strcmp(status["statusCode"].GetString(), "F") != 0 )
        {
            game->postponed = true;
            game->homescore = 0;
            game->awayscore = 0;
            continue;
        }
        
        game->homescore = home["score"].GetInt();
        game->awayscore = away["score"].GetInt();
        
        const Value& homerecord = home["leagueRecord"];
        const Value& awayrecord = away["leagueRecord"];
        
        game->homewins = homerecord["wins"].GetInt();
        game->homelosses = homerecord["losses"].GetInt();
        
        game->awaywins = awayrecord["wins"].GetInt();
        game->awaylosses = awayrecord["losses"].GetInt();
        
        
        //get decisions
        const Value& decisions = gameitem["decisions"];
        const Value& winner = decisions["winner"];
        
        game->winner = winner["fullName"].GetString();
        
        const Value& loser = decisions["loser"];
        game->loser = loser["fullName"].GetString();
        
        if( decisions.HasMember("save") )
        {
            const Value& save = decisions["save"];
            game->save = save["fullName"].GetString();
        }
        
        //content labels
        const Value& content = gameitem["content"];
        
        const Value& editorial = content["editorial"];
        const Value& recap = editorial["recap"];
        
        //use home values
        const Value& homerecap = recap["home"];
        const Value& headline = homerecap["headline"];
        
        const Value& subhead = homerecap["subhead"];
        
        if(subhead.GetStringLength() > 0)
        {
            game->lowerlabel =subhead.GetString();
        }
        else
        {
            const Value& title = homerecap["seoTitle"];
            
            game->lowerlabel =title.GetString();
        }
        
        game->upperlabel =  headline.GetString();
        
        const Value& photo = homerecap["photo"];
        const Value& cuts = photo["cuts"];
        const Value& img = cuts["400x224"];
        
        const Value& imgsrc = img["src"];
        game->photo = imgsrc.GetString();
    }
    
}

int CMLBJson::GetAwayTeamID(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& away = teams["away"];
    
    auto& team = away["team"];
    
    return team["id"].GetInt();
}

int CMLBJson::GetHomeTeamID(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];

    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& home = teams["home"];

    auto& team = home["team"];

    return team["id"].GetInt();
}

int CMLBJson::GetHomeScore(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];

    const Value& gameitem = games[game];
    
    //status
    const Value& status = gameitem["status"];
    if( strcmp(status["statusCode"].GetString(), "F") != 0 )
        return 0;
    
    const Value& teams = gameitem["teams"];
    const Value& home = teams["home"];

    return home["score"].GetInt();
}

int CMLBJson::GetHomeWins(int game)
{
    const Value& date = mDoc["dates"];
    
    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];

    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& home = teams["home"];
    const Value& record = home["leagueRecord"];
    
    return record["wins"].GetInt();
}

int CMLBJson::GetHomeLosses(int game)
{
    const Value& date = mDoc["dates"];
       
   //this is an array
   const Value& dateitem = date[0];
   const Value& games = dateitem["games"];

   const Value& gameitem = games[game];
   const Value& teams = gameitem["teams"];
   const Value& home = teams["home"];
   const Value& record = home["leagueRecord"];
   
   return record["losses"].GetInt();
}

int CMLBJson::GetAwayWins(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& away = teams["away"];
    const Value& record = away["leagueRecord"];
    
    
    return record["wins"].GetInt();
}

int CMLBJson::GetAwayLosses(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& away = teams["away"];
    const Value& record = away["leagueRecord"];
    
    
    return record["losses"].GetInt();
}

int CMLBJson::GetAwayScore(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    
    //status
    const Value& status = gameitem["status"];
    if( strcmp(status["statusCode"].GetString(), "F") != 0 )
        return 0;
    
    const Value& teams = gameitem["teams"];
    const Value& away = teams["away"];
    
    
    return away["score"].GetInt();
}

const char* CMLBJson::GetWinner(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    
    //get decisions
    const Value& decisions = gameitem["decisions"];
    const Value& winner = decisions["winner"];
    
    return winner["fullName"].GetString();
}

const char* CMLBJson::GetLoser(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    
    //get decisions
    const Value& decisions = gameitem["decisions"];
    const Value& loser = decisions["loser"];
    
    return loser["fullName"].GetString();
}

const char* CMLBJson::GetSave(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    
    //get decisions
    const Value& decisions = gameitem["decisions"];
    if( !decisions.HasMember("save") )
        return 0;
    
    const Value& save = decisions["save"];
    
    return save["fullName"].GetString();
}

const char* CMLBJson::GetUpperLabel(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& content = gameitem["content"];
    
    const Value& editorial = content["editorial"];
    const Value& recap = editorial["recap"];
    
    //use home values
    const Value& home = recap["home"];
    const Value& headline = home["headline"];
    
    return headline.GetString();
}

const char* CMLBJson::GetLowerLabel(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& content = gameitem["content"];
    
    const Value& editorial = content["editorial"];
    const Value& recap = editorial["recap"];
    
    //use home values
    const Value& home = recap["home"];
    const Value& subhead = home["subhead"];
    
    if(subhead.GetStringLength() > 0)
        return subhead.GetString();
    
    const Value& title = home["seoTitle"];
    
    return title.GetString();
}

int CMLBJson::GetGameCount()
{
    const Value& date = mDoc["dates"];
    if( date.Size() == 0)
        return 0;

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    return games.Size();
}
