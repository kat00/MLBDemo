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
    virtual ~CJsonQuery();

    void AddData(const char* data, size_t size);
    const char* Data() const { return m_document_data; }
    size_t Size() { return m_offset; }
    
protected:
    
    bool SendQuery(const char* query);
    

protected:

    rapidjson::Document mDoc;
    char* m_document_data;
    size_t m_doc_size;
    size_t m_offset;
};


#endif //DISNEYPROTO_CJSONQUERY_H
