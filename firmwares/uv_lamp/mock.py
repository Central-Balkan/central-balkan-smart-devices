from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import PlainTextResponse

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins='*',
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/config/get/")
async def read_item():
    return {
        "workTime": "5000",
        "restTime": "5000",
        "isCurrentlyWorkingInt": "0",
        "timeLeft": "3576"
    }


@app.options("/config/get/")
async def read_item():
    return {
        "workTime": "5000",
        "restTime": "5000",
        "isCurrentlyWorkingInt": "0",
        "timeLeft": "3576"
    }


@app.post("/config/set/")
async def read_item():
    return PlainTextResponse("Set config successfully")
