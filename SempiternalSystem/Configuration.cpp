#include "Configuration.h"

int Configuration::ParseKVP(const char* p_key, const char* p_value)
{
	for (int i = 0; i < keyValueListLength; ++i)
	{
		if (strcmp(p_key, KeyValueList[i].symbol))
		{
			if (KeyValueList[i].type == Type::Int)
			{
				*(int*)KeyValueList[i].dataPointer = TypeParser::string2int(p_value);
			}
			if (KeyValueList[i].type == Type::Bool)
			{
				if (strcmp("True", p_value))
				{
					*(bool*)KeyValueList[i].dataPointer = true;
				}
				else if(strcmp("False", p_value))
				{
					*(bool*)KeyValueList[i].dataPointer = false;
				}
				else
				{ }//UNDONE: throw exception
			}
			if (KeyValueList[i].type == Type::String)
			{
				*(char**)KeyValueList[i].dataPointer = (char*)p_value;
			}
			continue;
		}
		printf("Warning: Unrecognized parameter: %s\n", p_key);
	}
	return 0;
}

Configuration::Configuration() : videoHSize(640), videoVSize(480), telnetEnable(false)
{ }

int Configuration::LoadConfigurationFromFile(const char* p_fileName)
{
	FILE* cfg = fopen(p_fileName, "r");
	
	if (cfg == NULL)
	{
		int err = errno;
		fprintf(stderr, "Can't open config file:[%s], error code:%d\n", p_fileName, err);
		return -1;
	}

	char tmp;
	char key[32], value[128];
	bool isKeyField = true;
	char* keyP = key, * valueP = value;
	while (fscanf(cfg, "%c", &tmp))
	{
		if (tmp == '\r') continue;
		if(tmp == '\n')//do final check
		{
			*valueP = '\0';
			ParseKVP(key, value);
			keyP = key;
			valueP = value;
			continue;
		}
		if(tmp == '=')//if it is divider
		{
			if (isKeyField)//is it a real divider
			{
				isKeyField = false;//do field shift
				*keyP = '\0';
				continue;
			}
			*valueP++ = tmp;//parse as a value
		}
		if (isKeyField)
		{
			*keyP++ = tmp;
		}
		else
		{
			*valueP++ = tmp;
		}
	}
	fclose(cfg);
	return 0;
}