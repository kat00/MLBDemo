//
// Created by Kristina Theroux on 1/23/20.
//

#include <cstdio>
#include <iostream>
#include "CJsonQuery.h"
#include "constants.h"
#include "TcpConnection.h"
#include <fstream>
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/error/en.h"
#include <curl/curl.h>

using namespace rapidjson;

const char* QUERY_ADDR = "statsapi.mlb.com";
const char* QUERY_PATH = "/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=%d-%d-%d&sportId=1";

static std::string readBuffer;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    readBuffer.append((const char*)contents, realsize);
    return realsize;
}

CJsonQuery::CJsonQuery()
{
}

CJsonQuery::~CJsonQuery()
{
}

void CJsonQuery::SendRequest(int month, int day, int year)
{
    char path[256] = {0};
    snprintf(path, 255, QUERY_PATH, year, month, day);

    const int buffsize = 2048;
    char buffer[buffsize] = { 0 };
    snprintf(buffer, buffsize, "http://%s/%s", QUERY_ADDR,path);

    CURL *curl;
    CURLcode res;

    readBuffer.clear();

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, buffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    //TcpConnection conn;
    //conn.SetAddress(QUERY_ADDR);

    /*if(!conn.Open())
        return;

    Document doc;
    std::string json;
    if(conn.Write(buffer, strlen(buffer)) != 0)
    {
        memset(buffer, 0, buffsize);
        int contentlength = 0;
        int amountread = conn.Read(buffer, buffsize-1);

        bool hasstarted = false;
        while( amountread > 0)
        {
            if(!hasstarted)
            {
                //advance to json start
                char* jsonstart = strstr(buffer, "\r\n{");
                if(jsonstart)
                {
                    json.append(jsonstart);
                    hasstarted = true;
                }
            }
            else
            {
                json.append(buffer);
            }

            memset(buffer, 0, buffsize);
            amountread = conn.Read(buffer, buffsize-1);
        }
    }
    std::cout << "End " << std::endl;
    //std::cout << json << std::endl;*/

    //char filename[256] = { 0 };
    //snprintf(filename,256, "%d%d%d.json", month, day, year);
    
    //FILE* fp = fopen(filename, "r"); // non-Windows use "r"
    //char readBuffer[65536];
    //FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    ParseResult ok = mDoc.Parse(readBuffer.c_str());
    if( !mDoc.HasParseError() )
    {
        //test output for checking accuracy
        if( mDoc.IsObject() )
        {
            const Value& date = mDoc["dates"];
            if( date.Size() > 0)
            {
                //this is an array
                const Value& dateitem = date[0];
                const Value& games = dateitem["games"];
                //auto obj = date.GetObject();
                //const Value& games = obj["games"];

                int count = games.Size();
                for( int x = 0; x < count; ++x)
                {
                    const Value& gameitem = games[x];
                    const Value& teams = gameitem["teams"];
                    const Value& away = teams["away"];
                    const Value& home = teams["home"];

                    auto &team = home["team"];

                    std::cout << "Home: " << TeamNameFromID(team["id"].GetInt()) << std::endl;

                    auto &team2 = away["team"];
                    std::cout << "Away: " << TeamNameFromID(team2["id"].GetInt()) << std::endl;

                }
            }
            else
            {
                std::cout << "Mo Games" << std::endl;
            }
        }
    }
    else
    {
        fprintf(stderr, "JSON parse error: %s (%u)",
                GetParseError_En(ok.Code()), ok.Offset());

    }

}

int CJsonQuery::GetAwayTeamID(int game)
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

int CJsonQuery::GetHomeTeamID(int game)
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

int CJsonQuery::GetHomeScore(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];

    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& home = teams["home"];

    return home["score"].GetInt();
}

int CJsonQuery::GetHomeWins(int game)
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

int CJsonQuery::GetHomeLosses(int game)
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

int CJsonQuery::GetAwayWins(int game)
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

int CJsonQuery::GetAwayLosses(int game)
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

int CJsonQuery::GetAwayScore(int game)
{
    const Value& date = mDoc["dates"];

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    const Value& gameitem = games[game];
    const Value& teams = gameitem["teams"];
    const Value& away = teams["away"];
    
    
    return away["score"].GetInt();
}

const char* CJsonQuery::GetWinner(int game)
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

const char* CJsonQuery::GetLoser(int game)
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

const char* CJsonQuery::GetSave(int game)
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

const char* CJsonQuery::GetUpperLabel(int game)
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

const char* CJsonQuery::GetLowerLabel(int game)
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

int CJsonQuery::GetGameCount()
{
    const Value& date = mDoc["dates"];
    if( date.Size() == 0)
        return 0;

    //this is an array
    const Value& dateitem = date[0];
    const Value& games = dateitem["games"];
    
    return games.Size();
}
