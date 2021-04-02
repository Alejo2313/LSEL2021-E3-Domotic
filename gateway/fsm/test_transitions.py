from control_flow import Control_flow
import pytest

@pytest.fixture
def initial_fsm():
    fsm = Control_flow("GW")
    return fsm

def test_initial_state(initial_fsm):
    assert initial_fsm.state == "CONFIG"

def test_to_idle(initial_fsm):
    initial_fsm.start()
    assert initial_fsm.state == "IDLE"

def test_new_data(initial_fsm):
    initial_fsm.to_IDLE()
    initial_fsm.new_data()
    assert initial_fsm.state == "PROCESS_DATA"

def test_needResponse(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.need_response()
    assert initial_fsm.state == "PROCESS_DATA"

def test_no_need_response(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.no_need_command()
    assert initial_fsm.state == "IDLE"

def test_resp_ready(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.resp_ready()
    assert initial_fsm.state == "IDLE"

def test_new_order(initial_fsm):
    initial_fsm.to_IDLE()
    initial_fsm.new_order()
    assert initial_fsm.state == "PROCESS_ORDER"

def test_needData(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.need_data()
    assert initial_fsm.state == "PROCESS_ORDER"

def test_no_need_data(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.no_need_data()
    assert initial_fsm.state == "IDLE"

def test_data_ready(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.data_ready()
    assert initial_fsm.state == "IDLE"