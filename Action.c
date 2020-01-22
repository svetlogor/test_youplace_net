#include "stdlib.h"
#include "string.h"


#define LR_VARARGS_MAX 100
typedef struct { char *a[LR_VARARGS_MAX]; } t_lr_varargs;
t_lr_varargs f_args;
int f_i=0;

#define f_args_add(X) f_args.a[f_i++] = X

char *f_args_param_list[LR_VARARGS_MAX];
int f_args_param_idx = 0;

void f_args_add_paramarr(char *pattern, int i)
{
	char *p = (char*)malloc(strlen(pattern));
	sprintf(p, pattern, i);
	f_args_param_list[f_args_param_idx++] = p;
	f_args_add(p);
}

void f_args_reset()
{
	int i;
	for(i=0; i<f_args_param_idx; i++ ) 
	{
		if( f_args_param_list[i] ) 
		{
			free( f_args_param_list[i] );
		}
	}
	f_args_param_idx = 0;
	f_i = 0;
}

void param_radio()
{
	web_reg_save_param_attrib(
		"ParamName=RadioNames",
		"TagName=INPUT",
		"Extract=NAME",
		"Type=RADIO",
		"NotFound=warning",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		"IgnoreRedirections=Yes",
		LAST);
	 
	 
	web_reg_save_param_attrib(
		"ParamName=RadioValues",
		"TagName=INPUT",
		"Extract=VALUE",
		"Type=RADIO",
		"NotFound=warning",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		"IgnoreRedirections=Yes",
		LAST);
}

void param_select()
{
	web_reg_save_param_attrib(
		"ParamName=SelectsName",
		"TagName=SELECT",
		"Extract=NAME",
		"NotFound=warning",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		"IgnoreRedirections=Yes",
		LAST);
	
	web_reg_save_param_attrib(
		"ParamName=SelectsValue",
		"TagName=OPTION",
		"Extract=VALUE",
		"NotFound=warning",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		"IgnoreRedirections=Yes",
		LAST);
}

void param_text()
{
	web_reg_save_param_attrib(
		"ParamName=inputTexts",
		"TagName=INPUT",
		"Extract=NAME",
		"Type=TEXT",
		"NotFound=warning",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		"IgnoreRedirections=Yes",
		LAST);
}
 

Action()
{
	
	int StartPoint = 1;
	int EndPoint = 0;
	
	web_url("test.youplace.net", 
		"URL=http://test.youplace.net/", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t1.inf", 
		"Mode=HTML", 
		LAST);

	while ( EndPoint < 15 )
	{
	int CDs3_count=0; 
	int name_sel;
	int Count_Value_select=0;
	int i=1;
	int n=1;
	int y=0;
	int a[100];
	int max=0;
	int startSelector = 0;
	int startRadio = 0;
	int RadioName_count = 0;
	int equal_not_equal = 0;
	
	char SelectsValue[50];
	char *RadioNameFirst;
	char *RadioNameLast;
	char my_RN_value[100];
	char my_RV_value[100];
	
	lr_output_message("Question %d", StartPoint);
	EndPoint += 1;
	StartPoint += 1;
	
	
	if (EndPoint == 1)
	{	
	param_radio();

	param_select();
		
	param_text();
	
	lr_start_transaction("Start Button");

	web_link("Start test",
		"Text=Start test", 
		LAST);
	lr_end_transaction("Start Button", LR_AUTO);

	}
	
	f_args_add(ITEMDATA);
	
	lr_start_transaction("Search Radio");
	//  ÂÛÂÎÄ RADIO
	RadioName_count = atoi(lr_eval_string("{RadioNames_count}"));
	for (i=1; i<25; i++)
	{
		if (a[i])
		{
			a[i] = NULL;
		}
	}
	for( i=1; i <= RadioName_count; i++ )
	
	{
		if ( i == 1)
		{
			RadioNameFirst = lr_eval_string("{RadioNames_1}");
			startRadio = 1;
		}
		
		if ( i == RadioName_count )
		{
			sprintf( my_RV_value, "{RadioValues_%d}", i );
			a[i] = strlen(lr_eval_string(my_RV_value));
			max = startRadio;
			for (n=startRadio; n<30; n++)
			{
				if (a[n] > a[max]) 
				{
					max = n;
				}
			}
			f_args_add_paramarr("Name={RadioNames_%d}", i);
	        f_args_add_paramarr("Value={RadioValues_%d}", max);
	        f_args_add(ENDITEM);		
		}
		else
		{
			sprintf( my_RN_value, "{RadioNames_%d}", i );
			RadioNameLast = lr_eval_string(my_RN_value);
			for (y=0; RadioNameFirst[y] != '\0'; y++)
				{
					if ( RadioNameFirst[y] == RadioNameLast[y] )
					{ 
						equal_not_equal = 0;
					}
					else
					{
						equal_not_equal = 1;
					}
				}
			
			if ( equal_not_equal == 0)
			{
				sprintf( my_RV_value, "{RadioValues_%d}", i );
				a[i] = strlen(lr_eval_string(my_RV_value));
			}
			else
			{
				RadioNameFirst = lr_eval_string(my_RN_value);
				max = startRadio;
				for (n=startRadio; n<50; n++)
	    		{
					if (a[n] > a[max]) 
					{
						max = n;
					}
				}
				f_args_add_paramarr("Name={RadioNames_%d}", i-1);
	            f_args_add_paramarr("Value={RadioValues_%d}", max);
	            f_args_add(ENDITEM);
	            startRadio = i;
	            sprintf( my_RV_value, "{RadioValues_%d}", i );
				a[i] = strlen(lr_eval_string(my_RV_value));
	
			}
		}
	}
	lr_end_transaction("Search Radio", LR_AUTO);

	
	lr_start_transaction("Search Select");
	//	ÂÛÂÎÄ SELECT
	Count_Value_select = atoi(lr_eval_string("{SelectsValue_count}"));
	name_sel = 0;
	for (i=1; i<25; i++)
	{
		if (a[i])
		{
			a[i] = NULL;
		}
	}
	for(i=2; i<=Count_Value_select; i++)
	{
		sprintf(SelectsValue, "{SelectsValue_%d}", i );
		if(i == 2)
		{
			startSelector = 2;
		}
		if (strlen(lr_eval_string(SelectsValue))>0)
		{
			if (i == Count_Value_select)
			{
				a[i] = strlen(lr_eval_string(SelectsValue));
				name_sel = name_sel + 1;
				max = startSelector;
				for (n=startSelector; n<25; n++)
	    		{
					if (a[n] > a[max]) 
					{
						max = n;
					}
	    		}
				f_args_add_paramarr("Name={SelectsName_%d}", name_sel);
	            f_args_add_paramarr("Value={SelectsValue_%d}", max);
	            f_args_add(ENDITEM);		
			}
			else
			{
				a[i] = strlen(lr_eval_string(SelectsValue));
			}
		}
		else
		{
			name_sel = name_sel + 1;
			max = startSelector;
			for (n = startSelector; n < 25; n++)
    		{
				if (a[n] > a[max]) 
				{
					max = n;
				}
    		}
			f_args_add_paramarr("Name={SelectsName_%d}", name_sel);
            f_args_add_paramarr("Value={SelectsValue_%d}", max);
            f_args_add(ENDITEM);
			startSelector = i;		
		}
	}
	lr_end_transaction("Search Select", LR_AUTO);

	lr_start_transaction("Search Text");
	//	ÂÛÂÎÄ TEXT
	CDs3_count = atoi(lr_eval_string("{inputTexts_count}"));
	for(i=1; i<=CDs3_count; i++)
	{
        	f_args_add_paramarr("Name={inputTexts_%d}", i);
            f_args_add("Value=test");
            f_args_add(ENDITEM);


	}
	lr_end_transaction("Search Text", LR_AUTO);


	f_args_add(LAST);
    
    if (atoi(lr_eval_string ("{TestSuccessfully}"))> 0)
    {
		lr_output_message("Test successfully passed");
		break;
    }
    else
    {  
    	
		//RADIO			
		param_radio();
		
		//SELECT	
		param_select();
			
		//TEXT
		param_text();
		
		//Test successfully	
		web_reg_find("Text=Test successfully passed",
		     "SaveCount=TestSuccessfully",
		     LAST);
	
    
		web_submit_form("Question", f_args);
		f_args_reset();
    }
   }

	return 0;
}