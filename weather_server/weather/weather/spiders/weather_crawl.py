#-*- coding:utf-8 -*-

import scrapy
import re
import urllib
from ..items import WeatherItem
from bs4 import BeautifulSoup

class WeatherCrawl(scrapy.Spider):
	name = "weather"
	allowed_domains = ["www.sina.com"]
	start_urls = ["http://weather.sina.com.cn/xian"]

	def parse(self,response):
		print response 
		item = WeatherItem()
		item['city'] = response.xpath('//*[@id="slider_ct_name"]/text()').extract()
		item['up_date'] = response.xpath('//*[@class="slider_ct_date"]/text()').extract()
		item['up_time'] = response.xpath('//*[@class="slider_ct_time png24"]/text()').extract()
		tenDay = response.xpath('//*[@id="blk_fc_c0_scroll"]');
		item['date'] = tenDay.css('p.wt_fc_c0_i_date::text').extract()
		item['dayDesc'] = tenDay.css('img.icons0_wt::attr(title)').extract()
		item['dayTemp'] = tenDay.css('p.wt_fc_c0_i_temp::text').extract()
		return item

#	def parse(self,response):
#		html_doc = response.body
#		soup = BeautifulSoup(html_doc,"lxml")
#		itemTemp = {}
#		itemTemp['city'] = soup.find(id='slider_ct_name')
#		tenDay = soup.find(id='blk_fc_c0_scroll')
#		itemTemp['date'] = tenDay.findAll("p", {"class": 'wt_fc_c0_i_date'})
#		itemTemp['dayDesc'] = tenDay.findAll("img", {"class": 'icons0_wt'})
#		itemTemp['dayTemp'] = tenDay.findAll('p', {"class": 'wt_fc_c0_i_temp'})
#		item = WeatherItem()
#		for att in itemTemp:
#			item[att] = []
#			if att == 'city':
#				item[att]=itemTemp.get(att).text
#				continue
#			for obj in itemTemp.get(att):
#				if att == 'dayDesc':
#					item[att].append(obj['title'])
#				else:
#					item[att].append(obj.text)
#		return item
