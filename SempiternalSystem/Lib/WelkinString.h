/*struct westring {

};*/
#include<string>
#include<cctype>
#include<sstream>

#ifndef __WELKINSTRING_H_
#define __WELKINSTRING_H_

namespace WelkinEngine
{
	typedef std::string string;

	/*
	方法，将输入中的每一个字母转化为大写
	参数，待处理字符串
	*/
	inline string to_upper(const string& i_str)
	{
		string ans;
		for(unsigned int i = 0; i < i_str.size(); ++i)
		{
			if(i_str[i] >= 'a' && i_str[i] <= 'z')
			{
				ans.push_back( i_str[i] - 32 );
			}
		}
		return ans;
	}

	/*
	方法，将输入中的每一个字符转化为小写（如果可以转化）
	参数，待处理字符串
	*/
	inline string to_lower(const string& i_str)
	{
		string ans;
		for(unsigned int i = 0; i < i_str.size(); ++i)
		{
			if(i_str[i] >= 'A' && i_str[i] <= 'Z')
			{
				ans.push_back( i_str[i] + 32 );
			}
		}
		return ans;
	}

	/*
	方法，将一个整数转化为字符串
	参数，待处理整数
	*/
	inline string IntToString(int a)
	{
		//string str;
		//while(a != 0)
		//{
		//	char temp = (char)'0' + (a % 10);
		//	a /= 10;
		//	//char* it = &temp;
		//	str.push_back(temp);
		//}
		//return str;
		std::ostringstream stream;
		stream << a; //n为int类型
		return stream.str();
	}

	/*
	方法，取出字符串第x个单词（空格分隔级）
	参数，待处理字符串，x(1,2,3,...)
	*/
	inline string GetWord(const string& a, const int cnt)
	{
		int i, j = 0;
		while(a[j] != ' ' || a[j + 1] == ' ')//Skip Unspace Character
			++j;
		
		for(i = 1; i < cnt; ++i)
		{
			while(a[j] != ' ' || a[j + 1] == ' ')//Skip Unspace Character
				++j;
			++j;
		}
		string ans;
		while(a[j] != ' ' || a[j + 1] == ' ')
			ans.push_back(a[j++]);
		return ans;
	}

	/*
	方法，将字符串转化为整数
	说明，将所有数值拼接，略过非法字符。调用前请自行确定数值格式
	参数，要转化的字符串
	*/
	inline int StringToInt(const string a)
	{
		int ans = 0, i = 0, edge = a.size(), is_neg = 0;
		while(i < edge)
		{
			if(a[i] == '-') is_neg = 1;
			if(a[i] >= '0' && a[i] <= '9')
				ans = ans * 10 + a[i] - '0';
			++i;
		}
		return is_neg ? -ans : ans;
	}
}

#endif