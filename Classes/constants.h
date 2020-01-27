//
// Created by Kristina Theroux on 1/23/20.
//

#ifndef DISNEYPROTO_CONSTANTS_H
#define DISNEYPROTO_CONSTANTS_H

static const char* HTTP_GET_HEADER =
        "GET %s HTTP/1.1\r\n"      // path
        "Host: %s\r\n"             // host
        "Connection: keep-alive\r\n"       // keep-alive or close
        "\r\n";

const char* TeamNameFromID(int id);

void AdvanceDate(int& month, int& day, int& year);
void DecrementDate(int& month, int& day, int& year);

#endif //DISNEYPROTO_CONSTANTS_H
