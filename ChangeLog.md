
# v1.0.11(2024-09-09)
* 过滤掉前收盘价格等于0的数据

# v1.0.10(2023-12-20)
* 升级版本号

# v1.0.9(2023-12-19)
* 读取服务器中的港股代码(将近4000条合约)，再读取csv中的合约代码，双重保障

# v1.0.8(2023-06-06)
* 开盘前会收到昨天的最后一条数据，过滤掉

# v1.0.6(2023-05-17)
* 可能会乱序，过滤掉sum_amount变小的Tick，否则会影响amout的计算

# v1.0.5(2023-05-15)
* 增加new_amount与sum_amount

# v1.0.4(2023-05-15)
* 去掉open_interes字段; 结构体中无amout, 去掉new_amount与sum_amount
* 结构体中的CurrNumber字段，是几个tick共用的，new_volume内部维护，不再使用CurrNumber

# v1.0.2(2023-05-11)
通过读取服务器获取港股信息，暂是不用

# v1.0.1 (2023-05-09)
* api无成交金额，内部自己计算维护
* 合约代码通过配置文件获取
* Api是2.1版本，只有行情，没有交易


读取服务器指令
curl -H "Content-Type:application/json" -X POST -d '{"userID":"MN00000001","password":"888888"}' http://222.73.105.170:19999/account/login

curl -H "Content-Type:application/x-www-form-urlencoded" -H "token:eyJ0eXAiOiJKV1QiLCJ0eXBlIjoiSldUIiwiYWxnIjoiSFMyNTYifQ.eyJleHAiOjE2ODM4ODY3NjgsInVzZXJJRCI6Ik1OMDAwMDAwMDEiLCJpYXQiOjE2ODM4NzU5Njh9.hz73kIpBT-8Sh8JFeoti9_RKrlQH7_oZf6FzBYOPifU" -X POST -d "jsonText=%7B%22size%22%3A%2220%22%2C%22pageNo%22%3A%221%22%2C%22exCode%22%3A%22HKEX%22%2C%22prdCode%22%3A%22%22%2C%22exMarketType%22%3A%22S%22%7D" http://222.73.105.170:19999/contract/getContract

curl -H "Content-Type:application/x-www-form-urlencoded" -H "token:eyJ0eXAiOiJKV1QiLCJ0eXBlIjoiSldUIiwiYWxnIjoiSFMyNTYifQ.eyJleHAiOjE2ODM4ODY3NjgsInVzZXJJRCI6Ik1OMDAwMDAwMDEiLCJpYXQiOjE2ODM4NzU5Njh9.hz73kIpBT-8Sh8JFeoti9_RKrlQH7_oZf6FzBYOPifU" -X POST --data-urlencode 'jsonText={"size":"200","pageNo":"0","exCode":"HKEX","prdCode":"","exMarketType":"S"}' http://222.73.105.170:19999/contract/getContract

string str_json = "{\"size\":\"10000\",\"pageNo\":\"1\",\"exCode\":\"HKEX\",\"prdCode\":\"\",\"exMarketType\":\"S\"}";
string new_text = x::URL::Escape(str_json);
-d中的转码由此生成