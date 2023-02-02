from flask import request,Response
from configparser import ConfigParser
import base64
import requests
import time
import datetime
import json
import urllib.parse

api_key = 'otMs5VYi08Zwi1uCzM9WXNMG'
secret_key = 'Q59X67CqjKLLSfVxMFuw5ggSEMmUungQ'
error_id = 0
error_msg = ''

def get_token():
    url = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=' \
           + api_key + '&client_secret=' + secret_key
    tim = time.time()
    response = requests.post(url)
    if response:
        resp = response.json()
        token = resp['access_token']
        expire = resp['expires_in']
        return (token,expire,tim)

def check_token() -> str:
    cfg = ConfigParser()
    cfg.read('/data/cat/conf.ini')
    if not cfg.has_option('token','access_token') or not cfg.has_option('token','expire') or not cfg.has_option('token','time') \
      or float(time.time()) - float(cfg.get('token','time')) > float(cfg.get('token','expire')) * 0.9:
        token,expire,tim = get_token()
        if not cfg.has_section('token'):
            cfg.add_section('token')
        cfg.set('token', 'access_token', token)
        cfg.set('token', 'expire', str(expire))
        cfg.set('token', 'time', str(tim))
        with open('/data/cat/conf.ini','w') as file:
            cfg.write(file)
        return token
    else:
        return cfg.get('token','access_token')

def identify_image(token:str, image:str) -> float:
    global error_id, error_msg
    url = 'https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=' + token
    params = {'image': image}
    response = requests.post(url=url, data=params, headers={'content-type':'application/x-www-form-urlencoded'})
    if response:
        res = response.json()
        score = 0.0
        if 'result' not in res.keys():
            error_id = int(res['error_code'])
            error_msg = 'from baidu: ' + res['error_msg']
            return -1
        for item in res['result']:
            if '猫' in item['name']:
                score += float(item['score'])
        return score
    else:
        error_id = -1
        error_msg = 'no response'

def save_log(image:str, success=False, score:float=0):
    if success: log = '[SUCCESS]'
    else: log = '[FAILED]'
    ima = datetime.datetime.today()
    filename = ima.strftime('%Y-%m-%d-%H-%M-%S') + '.jpg'
    with open('/data/cat/log.txt','a') as f:
        log += 'filename=' + filename
        log += '; score=' + str(score)
        f.write(log)
    image = base64.b64decode(image)
    with open('/data/cat/images/'+filename, 'wb') as f:
        f.write(image)


def identify_image_main() -> str:
    token = check_token()
    if request.content_type == 'application/json':
        data = request.json
    elif request.content_type == 'application/x-www-form-urlencoded':
        data = request.args
    
    if 'code' not in data or data['code'] != 'RmFuaGFpNjY2QDIwMjI':
        params = {
            'error_id': -1,
            'error_msg': 'incorrect_code'
        }
        ret = str(json.dumps(params))
        return ret
    if 'image' not in data.keys():
        params = {
            'error_id': -2,
            'error_msg': 'param image not found'
        }
        ret = str(json.dumps(params))
        save_log(image)
        return ret
    
    image = data['image']
    image = urllib.parse.unquote_plus(image)
    result = identify_image(token, image)
    if result != -1:
        params = {
            'error_id': 0,
            'error_msg': 'ok',
            'score': result
        }
        save_log(image=image, success=True, score=result)
    else:
        global error_id, error_msg
        params = {
            'error_id': error_id,
            'error_msg': error_msg,
            'score': -1
        }
        save_log(image)
    return Response(json.dumps(params), mimetype='application/json')