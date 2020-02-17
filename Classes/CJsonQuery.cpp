//
// Created by Kristina Theroux on 1/23/20.
//

#include <cstdio>
#include <iostream>
#include "CJsonQuery.h"
#include "constants.h"
//#include "TcpConnection.h"
//#include <fstream>
#include <string>
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/error/en.h"
#include <curl/curl.h>

using namespace rapidjson;


const size_t ONE_MB = 1024 * 1024;
const size_t INITIAL_DOC_SIZE = ONE_MB * 6;

static std::string readBuffer;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *query)
{
    size_t realsize = size * nmemb;
    ((CJsonQuery*)query)->AddData((const char*)contents, realsize);
    return realsize;
}

bool  CJsonQuery::SendQuery(const char* query)
{
    CURL *curl;
    CURLcode res;
    
    //reset offset
    m_offset = 0;

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, query);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        return true;
    }
    
    return false;
    
}

void CJsonQuery::AddData(const char* data, size_t size)
{
    if( m_offset + size >= m_doc_size )
    {
        char* olddata = m_document_data;
        m_doc_size += ONE_MB;
        memset(m_document_data, 0, m_doc_size);
        memcpy(m_document_data, olddata, m_offset);
        
        delete [] olddata;
    }
    
    std::memcpy(m_document_data + m_offset, data, size);
    m_offset += size;
}

CJsonQuery::CJsonQuery()
: m_offset(0)
{
    m_document_data = new char[INITIAL_DOC_SIZE];
    memset(m_document_data, 0, INITIAL_DOC_SIZE);
    m_doc_size = INITIAL_DOC_SIZE;
}

CJsonQuery::~CJsonQuery()
{
    if( m_document_data )
        delete [] m_document_data;
}

