"""
Ukrainian texts word frequencies
Gets url list fron input XML file
"""

from requests import get
from requests.exceptions import RequestException
from contextlib import closing
from bs4 import BeautifulSoup

table = {}

def url_get_content(url_list):
	p_content = ""
	url_content = []
	for url in url_list.find_all('url'):
		try:
			with closing(get(url.text, stream=True)) as resp:
				if is_good_response(resp):
					for p in BeautifulSoup(resp.content, 'html.parser').find_all('p'):
						p_content += p.text
					url_content.append(split_string(p_content))

		except RequestException as e:
			log_error('Error during requests to {0} : {1}'.format(url, str(e)))
	return url_content

def is_good_response(resp):
	content_type = resp.headers['Content-Type'].lower()
	return (resp.status_code == 200
			and content_type is not None
			and content_type.find('html') > -1)

def log_error(e):
	print(e)

def split_string(string):
	word_array = []
	word = ""
	for i in range(len(string)):
		if (string[i] >= 'А' and string[i] <= 'Я') or (string[i] >= 'а' and string[i] <= 'я') or string[i] == '\'' or string[i] == 'і' or string[i] == 'ї' or string[i] == 'є':
			word += string[i]
		else:
			word = word.replace('\'', '')
			if word == '':
				continue
			word_array.append(word)
			word = ""
	return word_array

def make_table(word_array):
	global table
	for word in word_array:
		if table.get(word):
			table[word] += 1
		else:
			table[word] = 1
	return

url_list = BeautifulSoup(open('list.xml').read(), 'xml')
url_content = url_get_content(url_list)
for word_array in url_content:
	make_table(word_array)
print(table)
