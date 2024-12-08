"use client";

import { useEffect, useRef } from "react";
import { Route } from "@/lib/types";
import PopupMessage from "./popup-message";

interface PopupMessageClientProps {
    routes: Route[];
}

export default function PopupMessageEmitter({
    routes,
}: PopupMessageClientProps) {
    const modalRef = useRef<HTMLDialogElement>(null);

    useEffect(() => {
        if (routes.length === 0 && modalRef.current) {
            modalRef.current.showModal();
        }
    }, [routes]);

    return <PopupMessage ref={modalRef} />;
}
