#include "common.h"

int g_version = 230806;
String g_version_text = "Arduino AnalogDriver";
String g_date_text = "2023/8/24";

int g_analogValue_1_ = 0;
int g_analogValue_2_ = 0;
int g_analogValue_3_ = 0;
int g_analogValue_4_ = 0;

int g_maxValue_1_ = 255;
int g_minValue_1_ = 0;
int g_maxValue_2_ = 255;
int g_minValue_2_ = 0;
int g_maxValue_3_ = 255;
int g_minValue_3_ = 0;
int g_maxValue_4_ = 255;
int g_minValue_4_ = 0;

float g_speedf_ = 0.001f;

int16_t g_status_ = 0;
