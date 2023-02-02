from flask import Flask, render_template, abort, redirect, url_for, send_file
from jinja2.exceptions import TemplateNotFound
import modules.test as test
import modules.cat as cat

app = Flask(__name__)
app.config['DEBUG'] = True
@app.route('/')
def hello_world():
    return redirect('/index.html')

@app.route('/test-request', methods = ['POST','GET'])
def test_request():
    return test.test_request()

@app.route('/translate', methods = ['GET','POST'])
def baidu_translate():
    return test.baidu_translate()

@app.route('/error/<number>')
def num(number):
    try:
        abort(int(number))
    except LookupError:
        abort(404)

@app.route('/cat/identify/', methods = ['POST'])
def cat_identify():
    return cat.identify_image_main()

@app.route('/<name>.html')
def html(name):
    try:
        return render_template(name + '.html')
    except TemplateNotFound:
        abort(404)

@app.route('/favicon.ico')
def favicon():
    try:
        return send_file('./static/favicon.ico')
    except FileNotFoundError:
        abort(404)

if __name__ == '__main__':
    app.run()