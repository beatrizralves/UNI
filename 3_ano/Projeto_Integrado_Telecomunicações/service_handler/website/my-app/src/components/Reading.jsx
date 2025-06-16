

function Reading({id, value, timestamp}){


    return(
        <div className="space-x-10"> 
            <div className="text-xs inline"> #{id}</div>
            <div className="text-sm font-bold inline"> {value}</div>
            <div className="text-sm inline"> {timestamp}</div>
        </div>
    )

}

export default Reading