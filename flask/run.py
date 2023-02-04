from flask import Flask, request, abort
from datetime import datetime
import requests
import json
app = Flask(__name__)
recognized = False
app.config['DEBUG'] = True
@app.route('/', methods = ['POST'])
def get_photo():
    if(request.content_type != 'image/jpeg' or not request.data):
        abort(415)
    img = request.data
    time = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
    with open('F:\\code\\project\\cat-feeding\\images\\' + time + '.jpg', 'wb+') as file:
        file.write(img)
    ret = requests.post('http://127.0.0.1:24401', data=img).json()
    print(ret)
    confidence = 0.0
    if 'results' in ret:
        for line in ret['results']:
            confidence += line['confidence']
    global recognized
    recognized = (confidence >= 0.8)
    return 'success'

@app.route('/recognized', methods = ['GET', 'POST'])
def is_recognized():
    if recognized:
        return '1'
    else:
        return '0'

if __name__ == '__main__':
    app.run(host='0.0.0.0')