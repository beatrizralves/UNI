
import {Link} from 'react-router-dom'

import axios from 'axios'

function System({ id, name, state }) {

    
    function turnmeon(){
        axios.get(`http://localhost:8086/disconnect/${id}`);
        window.location.reload(false);
    }

    return (

        <div>
        {state =="ON" && (<div>
            <div className='p-2 rounded border-4 pb-2 mt-3'>
                <span className="text-xs font-bold mr-5 ml-1"> #{id} </span>
                <span className="text-xl"> {name} </span>
                <Link to='/sensors' state={{id}} >
                 {'(Visualizar sensores)'}
                </Link>        
                <input className="bg-red-400 ml-10 p-1 rounded text-white font-bold" type="button" value="End" onClick={turnmeon} />
            </div>
        </div>)}
        </div>
    )
}

export default System