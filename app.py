from flask import Flask, render_template, jsonify
import subprocess
import json

app = Flask(__name__)

def run_cpp_program(action=None):
    cmd = ["./main"]
    if action:
        cmd.append(action)
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    return json.loads(result.stdout)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/data')
def get_data():
    data = run_cpp_program()
    return jsonify(data)

@app.route('/api/process', methods=['POST'])
def process_order():
    result = run_cpp_program("process")
    return jsonify(result)

if __name__ == '__main__':
    app.run(debug=True, port=5000)