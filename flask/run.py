from flask import Flask, request, abort
app = Flask(__name__)
app.config['DEBUG'] = True
@app.route('/', methods = ['GET', 'POST'])
def hello_world():
    if(request.data):
        img = request.data
        with open('D:\\image.jpg', 'wb') as file:
            file.write(img)
        print('file wrote')
    if(request.content_type == 'image/jpeg'):
        return 'success'
    else:
        abort(415)

if __name__ == '__main__':
    app.run(host='0.0.0.0')