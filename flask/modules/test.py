from flask import request,render_template
import random
import requests
import json
import urllib.parse
import hashlib

def test_request():
    if request.method == 'POST':
        ret = 'POST\n'
        if request.content_type == 'text/plain':
            ret += request.data
        elif request.content_type == 'application/json':
            ret += str(request.data)
        else:
            for i in request.form.items():
                ret += f'{i[0]}={i[1]}\n'
        return ret
    elif request.method == 'GET':
        ret = '<strong>GET</strong><br>'
        for i in request.args.items():
            ret += f'{i[0]}={i[1]}<br>'
        return ret

url = 'https://fanyi-api.baidu.com/api/trans/vip/translate'
appid = '20211023000980346'
passwd = '0YIIHOdvJdieHkAFaTCw'

def baidu_translate():
    if request.method == 'GET':
        ret = render_template('translate.html')
        return ret
    elif request.method == 'POST':
        form = request.form
        return do_translate(from_lang=form['from-lang'],to_lang=form['to-lang'],text=form['text'])

def do_translate(from_lang, to_lang, text):
    salt = str(random.randrange(1000000000,9999999999))
    pre_sign = appid + text + salt + passwd
    sign = hashlib.md5(pre_sign.encode(encoding='utf-8')).hexdigest()
    params = {
        'q': text,
        'from': from_lang,
        'to': to_lang,
        'appid': appid,
        'salt': salt,
        'sign': sign
    }
    ret = requests.get(url=url, params=params)
    rett = json.loads(ret.text)
    translated = ''
    for res in rett['trans_result']:
        translated += urllib.parse.unquote(res['dst'])
    return translated