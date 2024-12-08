import { forwardRef, ForwardRefRenderFunction } from "react";

type PopupMessageProps = object;

const PopupMessage: ForwardRefRenderFunction<
    HTMLDialogElement,
    PopupMessageProps
> = (props, ref) => {
    return (
        <dialog ref={ref} id="my_modal_1" className="modal">
            <div className="modal-box">
                <h3 className="font-bold text-lg">No Route Found!</h3>
                <p className="py-4">
                    Press ESC key or click the button below to close
                </p>
                <div className="modal-action">
                    <form method="dialog">
                        <button className="btn">Close</button>
                    </form>
                </div>
            </div>
        </dialog>
    );
};

PopupMessage.displayName = "PopupMessage";

export default forwardRef(PopupMessage);
